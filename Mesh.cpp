#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <tiny_obj_loader.h>
#include <tiny_gltf.h>

float ucharToFloat(UCHAR a, UCHAR b, UCHAR c, UCHAR d) {
    float f;
    UCHAR c_arr[] = { a, b, c, d };
    memcpy(&f, &c_arr, sizeof(f));
    return f;
}

unsigned short int ucharToUint(UCHAR a, UCHAR b) {
    unsigned short int f;
    UCHAR c_arr[] = { a, b };
    memcpy(&f, &c_arr, sizeof(f));
    return f;
}

UINT maxOfThree(UINT a, UINT b, UINT c) {
    UINT max = a;

    if (max < b)
        max = b;
    if (max < c)
        max = c;

    return max;
}

Mesh::Mesh(const wchar_t* full_path) : Resource(full_path)
{
    tinyobj::attrib_t attribs;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::wstring input_file_temp(full_path);
    std::string input_file(input_file_temp.begin(), input_file_temp.end()), warn, err;
    std::filesystem::path filesystem_path = (std::filesystem::path)full_path;

    std::vector<VertexMesh> list_vertices;
    std::vector<unsigned int> list_indices;

    bool res = FALSE;
    if (filesystem_path.extension() == ".gltf" || filesystem_path.extension() == ".glb") {
        tinygltf::TinyGLTF loader;
        tinygltf::Model model;
        if (filesystem_path.extension() == ".gltf") {
            // Read the 3D model from a '.gltf' file
            res = loader.LoadASCIIFromFile(&model, &err, &warn, input_file);
        }
        else {
            // Read the 3D model from a '.glb' file
            res = loader.LoadBinaryFromFile(&model, &err, &warn, input_file);
        }
        if (!res) throw std::exception("Could not read Mesh from gltf file.");

        std::vector<Vec3> posVec, normalVec;
        std::vector<Vec2> texVec;
        size_t index_global_offset = 0, mat_id = 0;
        // Parse meshes
        for (size_t i = 0; i < model.meshes.size(); i++) {

            for (size_t j = 0; j < model.meshes[i].primitives.size(); j++) {
                tinygltf::Primitive primitive = model.meshes[i].primitives[j];

                for (auto& attrib : primitive.attributes) {
                    tinygltf::Accessor acc = model.accessors[attrib.second];
                    tinygltf::BufferView bfv = model.bufferViews[acc.bufferView];
                    tinygltf::Buffer buffer = model.buffers[bfv.buffer];

                    UINT jmp = 1;
                    if (acc.type == 2)
                        jmp = 8;
                    if (acc.type == 3)
                        jmp = 12;
                    
                    for (size_t k = bfv.byteOffset; k < bfv.byteOffset + bfv.byteLength; k+=jmp) {
                        if (attrib.first == "TEXCOORD_0") {
                            texVec.push_back(
                                Vec2(
                                    ucharToFloat(
                                        buffer.data[k],
                                        buffer.data[k + 1],
                                        buffer.data[k + 2],
                                        buffer.data[k + 3]
                                    ),
                                    ucharToFloat(
                                        buffer.data[k + 4],
                                        buffer.data[k + 5],
                                        buffer.data[k + 6],
                                        buffer.data[k + 7]
                                    )
                                )
                            );
                        }
                        if (attrib.first == "POSITION") {
                            posVec.push_back(
                                Vec3(
                                    -ucharToFloat(
                                        buffer.data[k],
                                        buffer.data[k + 1],
                                        buffer.data[k + 2],
                                        buffer.data[k + 3]
                                    ),
                                    ucharToFloat(
                                        buffer.data[k + 4],
                                        buffer.data[k + 5],
                                        buffer.data[k + 6],
                                        buffer.data[k + 7]
                                    ),
                                    ucharToFloat(
                                        buffer.data[k + 8],
                                        buffer.data[k + 9],
                                        buffer.data[k + 10],
                                        buffer.data[k + 11]
                                    )
                                )
                            );
                        }
                        if (attrib.first == "NORMAL") {
                            normalVec.push_back(
                                Vec3(
                                    ucharToFloat(
                                        buffer.data[k],
                                        buffer.data[k + 1],
                                        buffer.data[k + 2],
                                        buffer.data[k + 3]
                                    ),
                                    ucharToFloat(
                                        buffer.data[k + 4],
                                        buffer.data[k + 5],
                                        buffer.data[k + 6],
                                        buffer.data[k + 7]
                                    ),
                                    ucharToFloat(
                                        buffer.data[k + 8],
                                        buffer.data[k + 9],
                                        buffer.data[k + 10],
                                        buffer.data[k + 11]
                                    )
                                )
                            );
                        }
                    }
                }

                tinygltf::BufferView bvi = model.bufferViews[model.accessors[primitive.indices].bufferView];
                for (size_t k = bvi.byteOffset; k < bvi.byteLength + bvi.byteOffset; k += 2) {
                    list_indices.push_back(
                        ucharToUint(
                            model.buffers[bvi.buffer].data[k],
                            model.buffers[bvi.buffer].data[k + 1]
                        )
                    );
                }

                MaterialSlot mat_slot;
                mat_slot.StartIndex = index_global_offset;
                mat_slot.MaterialId = mat_id;
                mat_slot.NumIndices = list_indices.size();
                mMaterialSlots.push_back(mat_slot);

                index_global_offset += list_indices.size();
            }
            mat_id++;
        }

        // Save in the list_vertices
        size_t max_v = maxOfThree(posVec.size(), normalVec.size(), texVec.size());
        for (size_t i = 0; i < max_v; i++) {
            Vec3 pos;
            Vec3 normal;
            Vec2 tex;
            if (i < posVec.size()) {
                pos = posVec[i];
            }
            if (i < normalVec.size()) {
                normal = normalVec[i];
            }
            if (i < texVec.size()) {
                tex = texVec[i];
            }
            VertexMesh v_mesh(pos, tex, normal, Vec3(), Vec3());
            list_vertices.push_back(v_mesh);
        }
    }
    else if (filesystem_path.extension() == ".obj") {
        std::string mtldir = input_file.substr(0, input_file.find_last_of("\\/"));
        // Read the 3D Model from an '.obj' file
        res = tinyobj::LoadObj(
            &attribs,
            &shapes,
            &materials,
            &warn,
            &err,
            input_file.c_str(),
            mtldir.c_str()
        );
        if (!res) throw std::exception("Could not read Mesh from .obj file.");
  
        size_t size_vertex_index_lists = 0;
        for (auto& shape : shapes) {
            size_vertex_index_lists += shape.mesh.indices.size();
        }

        list_vertices.reserve(size_vertex_index_lists);
        list_indices.reserve(size_vertex_index_lists);
        mMaterialSlots.resize(materials.size());

        size_t index_global_offset = 0;

        // Store the data from the '.obj' file
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
                            Vec3(vx, vy, vz),  // position
                            Vec2(tx, ty),      // texcoord
                            Vec3(nx, ny, nz),  // normal
                            v_tangent,
                            v_binormal
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
    }

#if _DEBUG
    if (!err.empty()) {
        std::cout << "MESH ERR: " << err << std::endl;
        throw std::exception("Could not read Mesh from file.");
    }
    if (!warn.empty()) std::cout << "MESH WARN:" << warn << std::endl;
#endif

    void* shader_byte_code = nullptr;
    size_t size_shader = 0;
    // Retrieve the shader byte code of the 'VertexMeshLayoutShader.hlsl' compiled in GraphicsEngine constructor
    GraphicsEngine::get()->getVertexMeshLayoutShaderByteCodeAndSize(
        &shader_byte_code, &size_shader
    );
    // Create vertex and index buffer for the created mesh
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

    GraphicsEngine::get()->getVertexMeshLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader);
    mVertexBuffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(
        vertex_list_data,
        sizeof(VertexMesh),
        (UINT)vertex_list_size,
        shader_byte_code,
        (UINT)size_shader
    );
    mIndexBuffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(
        index_list_data,
        (UINT)index_list_size
    );

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
