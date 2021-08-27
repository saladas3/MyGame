#pragma once

#include "Prerequisites.h"
#include "RenderSystem.h"
#include "Texture.h"

/*
 * A device context is similar to a device, but it is responsible for managing the GPU
 * and the rendering pipeline (the device mostly handles video memory).
 * This object is used to render graphics and to determine how they will be rendered.
 */

class DeviceContext
{

public:
	DeviceContext(ID3D11DeviceContext* device_context, RenderSystem* system);
	~DeviceContext();

public:
	void clearRenderTargetColor(const SwapChainPtr& swap_chain, float red, float green, float blue, float alpha);
	
	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location);
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

	void clearRenderTargetColor(const TexturePtr& render_target, float red, float green, float blue, float alpha);
	void clearDepthStencil(const TexturePtr& depth_stencil);
	
	void setVertexBuffer(const VertexBufferPtr& vertex_buffer);
	
	void setVertexShader(const VertexShaderPtr& vertex_shader);
	void setPixelShader(const PixelShaderPtr& pixel_shader);
	
	void setTexture(const VertexShaderPtr& vertex_shader, const TexturePtr* texture, unsigned int num_textures);
	void setTexture(const PixelShaderPtr& pixel_shader, const TexturePtr* texture, unsigned int num_textures);

	void setConstantBuffer(const VertexShaderPtr& vertex_shader, const ConstantBufferPtr& buffer);
	void setConstantBuffer(const PixelShaderPtr& pixel_shader, const ConstantBufferPtr& buffer);
	
	void setIndexBuffer(const IndexBufferPtr& index_buffer);
	void setViewportSize(UINT width, UINT height);

private:
	ID3D11DeviceContext* mDeviceContext = nullptr;
	RenderSystem* mRenderSystem = nullptr;

private:
	friend class ConstantBuffer;

};

