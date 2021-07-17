#include "PixelShader.h"

PixelShader::PixelShader(const void* shader_byte_code, size_t byte_code_size, RenderSystem* system)
{
	mRenderSystem = system;
	if (!SUCCEEDED(mRenderSystem->mD3DDevice->CreatePixelShader(shader_byte_code, byte_code_size, nullptr, &mPs)))
		throw std::exception("PixelShader not created successfully");
}

PixelShader::~PixelShader()
{
	mPs->Release();
}
