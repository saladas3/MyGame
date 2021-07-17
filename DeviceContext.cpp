#include "DeviceContext.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* device_context, RenderSystem* system)
{
	mRenderSystem = system;
	mDeviceContext = device_context;
}

DeviceContext::~DeviceContext()
{
	mDeviceContext->Release();
}

void DeviceContext::clearRenderTargetColor(const SwapChainPtr& swap_chain, float red, float green, float blue, float alpha)
{
	FLOAT clear_color[] = { red, green, blue, alpha };
	mDeviceContext->ClearRenderTargetView(swap_chain->mRtv, clear_color);
	mDeviceContext->ClearDepthStencilView(swap_chain->mDsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	mDeviceContext->OMSetRenderTargets(1, &swap_chain->mRtv, swap_chain->mDsv);
}

void DeviceContext::drawTriangleList(UINT vertex_count, UINT start_vertex_index)
{
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location)
{
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->DrawIndexed(index_count, start_index_location, start_vertex_index);
}

void DeviceContext::drawTriangleStrip(UINT vertex_count, UINT start_vertex_index)
{
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	mDeviceContext->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::setVertexBuffer(const VertexBufferPtr& vertex_buffer)
{
	UINT stride = vertex_buffer->mSizeVertex;
	UINT offset = 0;
	mDeviceContext->IASetVertexBuffers(0, 1, &vertex_buffer->mBuffer, &stride, &offset);
	mDeviceContext->IASetInputLayout(vertex_buffer->mLayout);
}

void DeviceContext::setVertexShader(const VertexShaderPtr& vertex_shader)
{
	mDeviceContext->VSSetShader(vertex_shader->mVs, nullptr, 0);
}

void DeviceContext::setPixelShader(const PixelShaderPtr& pixel_shader)
{
	mDeviceContext->PSSetShader(pixel_shader->mPs, nullptr, 0);
}

void DeviceContext::setViewportSize(UINT width, UINT height)
{
	D3D11_VIEWPORT vp = {};
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	mDeviceContext->RSSetViewports(1, &vp);
}
