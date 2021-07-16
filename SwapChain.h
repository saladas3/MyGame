#pragma once

#include "RenderSystem.h"
#include "Prerequisites.h"

/*
 * The SwapChain is a collection of buffers that swap their values continuously
 * in order to prevent tearing. This is basically the resource that presents the images on the screen.
 */

class SwapChain
{

public:
	SwapChain(HWND hWnd, UINT width, UINT height, RenderSystem* renderSystem);
	~SwapChain();

private:
    void reloadBuffers(UINT width, UINT height);

public:
    void setFullScreen(bool fullscreen, unsigned int width, unsigned int height);
    void resize(unsigned int width, unsigned int height);
    bool present(bool vsync);

private:
    IDXGISwapChain* mSwapChain = nullptr;
    ID3D11RenderTargetView* mRtv = nullptr;
    ID3D11DepthStencilView* mDsv = nullptr;
    RenderSystem* mRenderSystem = nullptr;

private:
    friend class DeviceContext;

};

