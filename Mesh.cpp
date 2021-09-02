#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION

#include <tiny_obj_loader.h>

Mesh::Mesh(const wchar_t* full_path) : Resource(full_path)
{
    tinyobj::attrib_t attribs;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    std::wstring input_file_temp(full_path);
    std::string input_file(input_file_temp.begin(), input_file_temp.end());

    std::string mtldir = input_file.substr(0, input_file.find_last_of("\\/"));

    // Read the 3D Model from an '.obj' file
    bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &warn, &err, input_file.c_str(), mtldir.c_str());

    if (!err.empty() || !res) throw std::exception("Could not read from file or Mesh not created.");

    std::vector<VertexMesh> list_vertices;
    std::vector<unsigned int> list_indices;

    size_t size_vertex_index_lists = 0;

    for (auto& shape : shapes) {
        size_vertex_index_lists += shape.mesh.indices.size();
    }

    list_vertices.reserve(size_vertex_index_lists);
    list_indices.reserve(size_vertex_index_lists);

    mMaterialSlots.resize(materials.size());

    size_t index_global_offset = 0;

    // Save the data in the '.obj' file
    for (size_t m = 0; m < materials.size(); m++) {
        mMaterialSlots[m].StartIndex = index_global_offset;
        mMaterialSlots[m].MaterialId = m;

        for (auto& shape : shapes) {
            size_t index_offset = 0;

            for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
                unsigned char num_face_verts = shape.mesh.num_face_vertices[f];

                if (shape.mesh.material_ids[f] != m) {
                    index_offset += num_face_verts;
                    continue;
                }

                Vec3 vertices_face[3];
                Vec2 texcoords_face[3];

                for (unsigned char v = 0; v < num_face_verts; v++) {
                    tinyobj::index_t index = shape.mesh.indices[index_offset + v];

                    tinyobj::real_t vx = attribs.vertices[(size_t)(index.vertex_index) * 3 + 0];
                    tinyobj::real_t vy = attribs.vertices[(size_t)(index.vertex_index) * 3 + 1];
                    tinyobj::real_t vz = attribs.vertices[(size_t)(index.vertex_index) * 3 + 2];

                    tinyobj::real_t tx = 0, ty = 0;
                    if (!attribs.texcoords.empty()) {
                        tx = attribs.texcoords[(size_t)(index.texcoord_index) * 2 + 0];
                        ty = attribs.texcoords[(size_t)(index.texcoord_index) * 2 + 1];
                    }

                    vertices_face[v] = Vec3(vx, vy, vz);
                    texcoords_face[v] = Vec2(tx, ty);
                }

                Vec3 tangent, binormal;

                this->computeTangents(
                    vertices_face[0], vertices_face[1], vertices_face[2],
                    texcoords_face[0], texcoords_face[1], texcoords_face[2],
                    tangent, binormal
                );

                for (unsigned char v = 0; v < num_face_verts; v++) {
                    tinyobj::index_t index = shape.mesh.indices[index_offset + v];

                    tinyobj::real_t vx = attribs.vertices[(size_t)(index.vertex_index) * 3 + 0];
                    tinyobj::real_t vy = attribs.vertices[(size_t)(index.vertex_index) * 3 + 1];
                    tinyobj::real_t vz = attribs.vertices[(size_t)(index.vertex_index) * 3 + 2];

                    tinyobj::real_t tx = 0, ty = 0;
                    if (!attribs.texcoords.empty()) {
                        tx = attribs.texcoords[(size_t)(index.texcoord_index) * 2 + 0];
                        ty = attribs.texcoords[(size_t)(index.texcoord_index) * 2 + 1];
                    }

                    tinyobj::real_t nx = 0, ny = 0, nz = 0;
                    if (!attribs.normals.empty()) {
                        nx = attribs.normals[(size_t)(index.normal_index) * 3 + 0];
                        ny = attribs.normals[(size_t)(index.normal_index) * 3 + 1];
                        nz = attribs.normals[(size_t)(index.normal_index) * 3 + 2];
                    }

                    Vec3 v_tangent, v_binormal;
                    v_binormal = Vec3::cross(Vec3(nx, ny, nz), tangent);
                    v_tangent = Vec3::cross(v_binormal, Vec3(nx, ny, nz));

                    VertexMesh vertex(
                        Vec3(vx, vy, vz), Vec2(tx, ty), Vec3(nx, ny, nz),
                        v_tangent, v_binormal
                    );

                    list_vertices.push_back(vertex);
                    list_indices.push_back((unsigned int)index_global_offset + v);
                }

                index_offset += num_face_verts;
                index_global_offset += num_face_verts;
            }
        }

        mMaterialSlots[m].NumIndices = index_global_offset - mMaterialSlots[m].StartIndex;

    }

    void* shader_byte_code = nullptr;
    size_t size_shader = 0;

    // Retrieve the shader byte code of the 'VertexMeshLayoutShader.hlsl' compiled in GraphicsEngine constructor
    GraphicsEngine::get()->getVertexMeshLayoutShaderByteCodeAndSize(
        &shader_byte_code, &size_shader
    );
    mVertexBuffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(
        &list_vertices[0],
        sizeof(VertexMesh),
        (UINT)list_vertices.size(),
        shader_byte_code,
        (UINT)size_shader
    );
    mIndexBuffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(
        &list_indices[0],
        (UINT)list_indices.size()
    );

    mRadius = this->boundingSphereRadius(list_vertices);
}

Mesh::Mesh(
    VertexMesh* vertex_list_data,
    unsigned int vertex_list_size,
    unsigned int* index_list_data,
    unsigned int index_list_size,
    MaterialSlot* material_slot_list,
    unsigned int material_slot_list_size
) : Resource(L"")
{
    void* shader_byte_code = nullptr;
    size_t size_shader = 0;

    // mRadius = this->boundingSphereRadius(list_vertices);

    GraphicsEngine::get()->getVertexMeshLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader);
    mVertexBuffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(vertex_list_data, sizeof(VertexMesh),
        (UINT)vertex_list_size,
        shader_byte_code,
        (UINT)size_shader);
    mIndexBuffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(index_list_data,
        (UINT)index_list_size);

    mMaterialSlots.resize(material_slot_list_size);

    for (unsigned int i = 0; i < material_slot_list_size; i++) {
        mMaterialSlots[i] = material_slot_list[i];
    }
}

Mesh::~Mesh() = default;

const VertexBufferPtr& Mesh::getVertexBuffer()
{
    return mVertexBuffer;
}

const IndexBufferPtr& Mesh::getIndexBuffer()
{
    return mIndexBuffer;
}

const MaterialSlot& Mesh::getMaterialSlot(unsigned int slot)
{
    return mMaterialSlots[slot];
}

size_t Mesh::getNumMaterialSlots()
{
    return mMaterialSlots.size();
}

void Mesh::computeTangents(const Vec3& v0, const Vec3& v1, const Vec3& v2, const Vec2& t0, const Vec2& t1, const Vec2& t2, Vec3& tangent, Vec3& binormal)
{
    Vec3 deltaPos1 = v1 - v0;
    Vec3 deltaPos2 = v2 - v0;

    Vec2 deltaUV1 = t1 - t0;
    Vec2 deltaUV2 = t2 - t0;

    // float r = 1.0f / (deltaUV1.m_x * deltaUV2.m_y - deltaUV1.m_y * deltaUV2.m_x);
    tangent = (deltaPos1 * deltaUV2.m_y - deltaPos2 * deltaUV1.m_y);
    tangent = Vec3::normalize(tangent);
    binormal = (deltaPos2 * deltaUV1.m_x - deltaPos1 * deltaUV2.m_x);
    binormal = Vec3::normalize(binormal);
}

float Mesh::boundingSphereRadius(const std::vector<VertexMesh>& list_vertices)
{
    return 0.0f;
}
