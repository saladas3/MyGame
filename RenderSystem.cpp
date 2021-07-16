#include "RenderSystem.h"

RenderSystem::RenderSystem()
{
    /*
     * Documentation
     * https://docs.microsoft.com/en-us/windows/win32/api/d3dcommon/ne-d3dcommon-d3d_driver_type
     *
     * D3D_DRIVER_TYPE_HARDWARE
     * A hardware driver, which implements Direct3D features in hardware.
     * This is the primary driver that you should use in your Direct3D
     * applications because it provides the best performance.
     * A hardware driver uses hardware acceleration (on supported hardware)
     * but can also use software for parts of the pipeline that are not supported in hardware.
     * This driver type is often referred to as a hardware abstraction layer or HAL.
     *
     * D3D_DRIVER_TYPE_REFERENCE
     * A reference driver, which is a software implementation that supports every Direct3D feature.
     * Used for testing, debugging or verifying bugs in other drivers.
     *
     * D3D_DRIVER_TYPE_WARP
     * A WARP driver, which is a high-performance software rasterizer.
     * The rasterizer supports feature levels 9_1 through level 10_1 with a high performance software implementation.
     * https://docs.microsoft.com/en-us/windows/win32/direct3darticles/directx-warp
     */

    D3D_FEATURE_LEVEL feature_levels[] =
    {
            D3D_FEATURE_LEVEL_11_0
    };

    /*
     * Feature levels
     * https://docs.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-devices-downlevel-intro
     * Using feature levels, you can develop an application for Direct3D 9, Microsoft Direct3D 10, or Direct3D 11,
     * and then run it on 9, 10 or 11 hardware.
     * (with some exceptions; for example, new 11 features will not run on an existing 9 card)
     */

    HRESULT res = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, NULL, feature_levels, ARRAYSIZE(feature_levels),
        D3D11_SDK_VERSION, &mD3DDevice, &mFeatureLevel, &mImmContext);

    if (FAILED(res)) throw std::exception("RenderSystem creation error.");

    mImmDeviceContext = std::make_shared<DeviceContext>(mImmContext, this);

    mD3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&mDxgiDevice);
    mDxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&mDxgiAdapter);
    mDxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&mDxgiFactory);

    initRasterizerState();
}

RenderSystem::~RenderSystem()
{
	if (mVs) mVs->Release();
	if (mPs) mPs->Release();

	if (mVsBlob) mVsBlob->Release();
	if (mPsBlob) mPsBlob->Release();

	mDxgiDevice->Release();
	mDxgiAdapter->Release();
	mDxgiFactory->Release();

	mD3DDevice->Release();
}

SwapChainPtr RenderSystem::createSwapChain(HWND hwnd, UINT width, UINT height)
{
    SwapChainPtr sc = nullptr;

    try {
        sc = std::make_shared<SwapChain>(hwnd, width, height, this);
    }
    catch (...) {}

    return sc;
}

DeviceContextPtr RenderSystem::getImmediateDeviceContext()
{
    return this->mImmDeviceContext;
}

void RenderSystem::initRasterizerState()
{
    D3D11_RASTERIZER_DESC desc = {};

    desc.DepthClipEnable = true; // Enable clipping based on distance.
    desc.FillMode = D3D11_FILL_SOLID; // Fill the triangles formed by the vertices. Adjacent vertices are not drawn

    // Cull mode indicates triangles facing the specified direction to not be drawn.
    desc.CullMode = D3D11_CULL_FRONT; // Do not draw triangles that are front-facing
    mD3DDevice->CreateRasterizerState(&desc, &mCullFrontState);
    desc.CullMode = D3D11_CULL_BACK; // Do not draw triangles that are back-facing
    mD3DDevice->CreateRasterizerState(&desc, &mCullBackState);
}