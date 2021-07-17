#include "VertexShader.h"

VertexShader::VertexShader(const void* shader_byte_code, size_t byte_code_size, RenderSystem* system)
{
	mRenderSystem = system;
	HRESULT res = mRenderSystem->mD3DDevice->CreateVertexShader(shader_byte_code, byte_code_size, nullptr, &mVs);
	if (!SUCCEEDED(res))
		throw std::exception("VertexShader could not be created.");
}

VertexShader::~VertexShader()
{
	mVs->Release();
}
