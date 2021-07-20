#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(void* list_indices, UINT size_list, RenderSystem* system)
{
    mRenderSystem = system;

    if (mBuffer) mBuffer->Release();

    D3D11_BUFFER_DESC buff_desc = {};
    buff_desc.Usage = D3D11_USAGE_DEFAULT;
    buff_desc.ByteWidth = 4 * size_list;
    buff_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    buff_desc.CPUAccessFlags = 0;
    buff_desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA init_data = {};
    init_data.pSysMem = list_indices;

    mSizeList = size_list;

    if (FAILED(mRenderSystem->mD3DDevice->CreateBuffer(&buff_desc, &init_data, &mBuffer)))
        throw std::exception("IndexBuffer not created successfully");
}

IndexBuffer::~IndexBuffer()
{
    mBuffer->Release();
}

UINT IndexBuffer::getSizeIndexList() const
{
    return this->mSizeList;
}
