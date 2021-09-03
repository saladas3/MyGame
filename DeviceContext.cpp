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

// --------------------------------------------------
// Draw functions -> executes every stage in the graphics pipeline when Draw() is called
// The triangles are drawn in a clockwise orientation
// It must be clockwise in order the know if the triangles are facing towards the camera
// --------------------------------------------------
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
// --------------------------------------------------

void DeviceContext::clearRenderTargetColor(const TexturePtr& render_target, float red, float green, float blue, float alpha)
{
	if (render_target->mType != Texture::Type::RenderTarget) return;
	FLOAT clear_color[] = { red, green, blue, alpha };
	mDeviceContext->ClearRenderTargetView(render_target->mRenderTargetView, clear_color);
}

void DeviceContext::clearDepthStencil(const TexturePtr& depth_stencil)
{
	if (depth_stencil->mType != Texture::Type::DepthStencil) return;
	mDeviceContext->ClearDepthStencilView(depth_stencil->mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void DeviceContext::setRenderTarget(const TexturePtr& render_target, const TexturePtr& depth_stencil)
{
	if (render_target->mType != Texture::Type::RenderTarget) return;
	if (depth_stencil->mType != Texture::Type::DepthStencil) return;
	mDeviceContext->OMSetRenderTargets(
		1,
		&render_target->mRenderTargetView,
		depth_stencil->mDepthStencilView
	);
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

void DeviceContext::setTexture(const VertexShaderPtr& vertex_shader, const TexturePtr* texture, unsigned int num_textures)
{
	ID3D11ShaderResourceView* list_res[32];
	ID3D11SamplerState* list_sampler[32];
	for (UINT i = 0; i < num_textures; i++)
	{
		list_res[i] = texture[i]->mShaderResView;
		list_sampler[i] = texture[i]->mSamplerState;
	}
	mDeviceContext->VSSetShaderResources(0, num_textures, list_res);
	mDeviceContext->VSSetSamplers(0, num_textures, list_sampler);
}

void DeviceContext::setTexture(const PixelShaderPtr& pixel_shader, const TexturePtr* texture, unsigned int num_textures)
{
	ID3D11ShaderResourceView* list_res[32];
	ID3D11SamplerState* list_sampler[32];
	for (UINT i = 0; i < num_textures; i++)
	{
		list_res[i] = texture[i]->mShaderResView;
		list_sampler[i] = texture[i]->mSamplerState;
	}
	mDeviceContext->PSSetShaderResources(0, num_textures, list_res);
	mDeviceContext->PSSetSamplers(0, num_textures, list_sampler);
}

void DeviceContext::setConstantBuffer(const VertexShaderPtr& vertex_shader, const ConstantBufferPtr& buffer)
{
	mDeviceContext->VSSetConstantBuffers(0, 1, &buffer->mBuffer);
}

void DeviceContext::setConstantBuffer(const PixelShaderPtr& pixel_shader, const ConstantBufferPtr& buffer)
{
	mDeviceContext->PSSetConstantBuffers(0, 1, &buffer->mBuffer);
}

void DeviceContext::setIndexBuffer(const IndexBufferPtr& index_buffer)
{
	mDeviceContext->IASetIndexBuffer(index_buffer->mBuffer, DXGI_FORMAT_R32_UINT, 0);
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
