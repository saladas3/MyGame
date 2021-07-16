#pragma once

#include "Prerequisites.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"

class RenderSystem
{

public:
	// Init the Render System and DirectX 11 Device
	RenderSystem();
	// Release all the resources loaded
	~RenderSystem();

public:
	SwapChainPtr createSwapChain(HWND hwnd, UINT width, UINT height);
    VertexBufferPtr createVertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list,
        void* shader_byte_code, UINT size_byte_shader);
    DeviceContextPtr getImmediateDeviceContext();

    // Method used to compile a shader file (.hlsl file)
    bool compileShaderFromFile(const wchar_t* file_name, const char* entry_point_name, LPCSTR target,
        void** shader_byte_code, size_t* byte_code_size);
    void releaseCompiledShader();
    void setRasterizerState(bool cull_front);

private:
    void initRasterizerState();

private:
    // DirectX Graphics Infrastructure (DXGI)
    // Members that start with I (Ex: ID3D11Device) are COM (Component Object Model) objects

    ID3D11Device* mD3DDevice = nullptr;
    // Initialized mFeatureLevel to remove the warning. Remove initialization of it produces errors.
    D3D_FEATURE_LEVEL mFeatureLevel = D3D_FEATURE_LEVEL_11_0;
    IDXGIDevice* mDxgiDevice = nullptr;

    // This is a value that indicates what graphics adapter Direct3D should use.
    // A graphics adapter typically refers to a GPU and its video memory, digital-to-analog converter, etc.
    IDXGIAdapter* mDxgiAdapter = nullptr;

    // An IDXGIFactory interface implements methods for generating DXGI objects (which handle full screen transitions).
    IDXGIFactory* mDxgiFactory = nullptr;

    ID3D11DeviceContext* mImmContext = nullptr;
    ID3DBlob* mBlob = nullptr;
    ID3D11RasterizerState* mCullFrontState = nullptr;
    ID3D11RasterizerState* mCullBackState = nullptr;
    ID3DBlob* mVsBlob = nullptr;
    ID3DBlob* mPsBlob = nullptr;
    ID3D11VertexShader* mVs = nullptr;
    ID3D11PixelShader* mPs = nullptr;

private:
    DeviceContextPtr mImmDeviceContext;

private:
	friend class SwapChain;
    friend class VertexBuffer;

};

