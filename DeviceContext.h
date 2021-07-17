#pragma once

#include "Prerequisites.h"
#include "RenderSystem.h"
#include "VertexBuffer.h"

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
	void setVertexBuffer(const VertexBufferPtr& vertex_buffer);
	void setVertexShader(const VertexShaderPtr& vertex_shader);
	void setPixelShader(const PixelShaderPtr& pixel_shader);
	void setViewportSize(UINT width, UINT height);

private:
	ID3D11DeviceContext* mDeviceContext = nullptr;
	RenderSystem* mRenderSystem = nullptr;

};

