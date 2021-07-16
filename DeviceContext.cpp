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
