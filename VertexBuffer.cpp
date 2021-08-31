#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list,
    void* shader_byte_code, size_t size_byte_shader, RenderSystem* system)
{
    mRenderSystem = system;

    D3D11_BUFFER_DESC buff_desc = {};
    buff_desc.Usage = D3D11_USAGE_DEFAULT;
    buff_desc.ByteWidth = size_vertex * size_list;
    buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buff_desc.CPUAccessFlags = 0;
    buff_desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA init_data = {};
    init_data.pSysMem = list_vertices;

    mSizeVertex = size_vertex;
    mSizeList = size_list;

    if (FAILED(mRenderSystem->mD3DDevice->CreateBuffer(&buff_desc, &init_data, &mBuffer)))
        throw std::exception("VertexBuffer was not created.");

    // Properties for each vertex
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        // Semantic name - semantic index - format - input slot - aligned byte offset - input slot class - instance data step rate
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
        // The aligned byte offset is 12 for TEXCOORD because it's the sum of the previous attribute (32b -> 4B * 3)
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    if (FAILED(mRenderSystem->mD3DDevice->CreateInputLayout(layout, ARRAYSIZE(layout), shader_byte_code, size_byte_shader, &mLayout)))
        throw std::exception("Input layout was not created.");
}

VertexBuffer::~VertexBuffer()
{
    this->mLayout->Release();
    this->mBuffer->Release();
}

UINT VertexBuffer::getSizeVertexList()
{
    return this->mSizeList;
}
