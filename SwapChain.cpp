#include "SwapChain.h"

SwapChain::SwapChain(HWND hWnd, UINT width, UINT height, RenderSystem* renderSystem) : mRenderSystem(renderSystem)
{
    DXGI_SWAP_CHAIN_DESC desc;

    // Clear out the memory for the struct to use
    ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

    // TODO: Parameterize the descriptor from user input
    desc.BufferCount = 1;                                   // one back buffer
    desc.BufferDesc.Width = width;
    desc.BufferDesc.Height = height;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
    desc.OutputWindow = hWnd;                               // the window to be used
    desc.SampleDesc.Count = 4;                              // nr. of multisamples (for anti-aliasing)
    desc.SampleDesc.Quality = 0;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    /*
     * DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
     * Set this flag to enable an application to switch modes by calling IDXGISwapChain::ResizeTarget.
     * When switching from windowed to full-screen mode, the display mode (or monitor resolution)
     * will be changed to match the dimensions of the application window.
     */
    desc.Windowed = TRUE;                                   // windowed / full-screen mode

    // Create the swap chain for the window indicated by HWND parameter
    HRESULT hr = mRenderSystem->mDxgiFactory->CreateSwapChain(mRenderSystem->mD3DDevice, &desc, &mSwapChain);

    if (FAILED(hr))
        throw std::exception("SwapChain not created successfully");

    reloadBuffers(width, height);
}

SwapChain::~SwapChain()
{
    mRtv->Release();
    // The release of depth stencil view was not here originally. Added by me, remove if it gives errors
    mDsv->Release();
    mSwapChain->Release();
}

void SwapChain::reloadBuffers(UINT width, UINT height)
{
    ID3D11Device* device = mRenderSystem->mD3DDevice;

    // Get the backBuffer color and create its render target view
    ID3D11Texture2D* backBuffer = nullptr;
    /*
    * __uuidof Operator -> Retrieves the GUID attached to the expression.
    * A GUID identifies an object such as a COM interfaces, or a COM class object, or a manager entry-point vector (EPV)
    * A GUID is a 128-bit value
    */
    HRESULT hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

    if (FAILED(hr)) throw std::exception("SwapChain not created successfully");

    // The render target is like a blank canvas where we draw the shapes
    // (in this case the canvas would be the back buffer of the swap chain)
    hr = device->CreateRenderTargetView(backBuffer, nullptr, &mRtv);
    backBuffer->Release();
    if (FAILED(hr)) throw std::exception("SwapChain not created successfully");

    D3D11_TEXTURE2D_DESC tex_desc = {};
    tex_desc.Width = width;
    tex_desc.Height = height;
    tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    tex_desc.Usage = D3D11_USAGE_DEFAULT;
    tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    tex_desc.MipLevels = 1;
    tex_desc.SampleDesc.Count = 1;
    tex_desc.SampleDesc.Quality = 0;
    tex_desc.MiscFlags = 0;
    tex_desc.ArraySize = 1;
    tex_desc.CPUAccessFlags = 0;

    hr = device->CreateTexture2D(&tex_desc, nullptr, &backBuffer);
    if (FAILED(hr)) throw std::exception("SwapChain not created successfully");

    hr = device->CreateDepthStencilView(backBuffer, nullptr, &mDsv);
    if (FAILED(hr)) throw std::exception("SwapChain not created successfully");

    backBuffer->Release();
}

void SwapChain::setFullScreen(bool fullscreen, unsigned int width, unsigned int height)
{
    resize(width, height);
    mSwapChain->SetFullscreenState(fullscreen, nullptr);
}

void SwapChain::resize(unsigned int width, unsigned int height)
{
    if (mRtv) mRtv->Release();
    if (mDsv) mDsv->Release();

    mSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    reloadBuffers(width, height);
}

bool SwapChain::present(bool vsync)
{
    mSwapChain->Present(vsync, NULL);
    return true;
}
