#pragma once

#include "Prerequisites.h"
#include "RenderSystem.h"

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

private:
	ID3D11DeviceContext* mDeviceContext = nullptr;
	RenderSystem* mRenderSystem = nullptr;

};

