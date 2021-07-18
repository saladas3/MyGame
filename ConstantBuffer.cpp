#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(void* buffer, UINT size_buffer, RenderSystem* system)
{
	mRenderSystem = system;

    D3D11_BUFFER_DESC buff_desc = {};
    buff_desc.Usage = D3D11_USAGE_DEFAULT;
    buff_desc.ByteWidth = size_buffer;
    buff_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    buff_desc.CPUAccessFlags = 0;
    buff_desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA init_data = {};
    init_data.pSysMem = buffer;

    if (FAILED(mRenderSystem->mD3DDevice->CreateBuffer(&buff_desc, &init_data, &mBuffer)))
        throw std::exception("ConstantBuffer was not created.");

}

ConstantBuffer::~ConstantBuffer()
{
    if (mBuffer) mBuffer->Release();
}

void ConstantBuffer::update(const DeviceContextPtr& context, void* buffer)
{
    context->mDeviceContext->UpdateSubresource(this->mBuffer, NULL, nullptr, buffer, NULL, NULL);
}
