#pragma once

#include "Prerequisites.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"

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
        void* shader_byte_code, size_t size_byte_shader);
    VertexShaderPtr createVertexShader(const void* shader_byte_code, size_t byte_code_size);
    PixelShaderPtr createPixelShader(const void* shader_byte_code, size_t byte_code_size);
    ConstantBufferPtr createConstantBuffer(void* buffer, UINT size_buffer);
    IndexBufferPtr createIndexBuffer(void* list_indices, UINT size_list);

public:
    DeviceContextPtr getImmediateDeviceContext();

    // Methods used to compile a shader file (.hlsl file)
    bool compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
    bool compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
    void releaseCompiledShader();
    void setRasterizerState(bool cull_front);

private:
    void initRasterizerState();

private:
    // DirectX Graphics Infrastructure (DXGI)
    // Members that start with I (Ex: ID3D11Device) are COM (Component Object Model) objects

    ID3D11Device* mD3DDevice = nullptr;
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

private:
    DeviceContextPtr mImmDeviceContext;

private:
	friend class SwapChain;
    friend class VertexBuffer;
    friend class VertexShader;
    friend class PixelShader;
    friend class ConstantBuffer;
    friend class IndexBuffer;
    friend class Texture;

};

