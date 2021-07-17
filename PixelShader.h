#pragma once

#include "Prerequisites.h"
#include "RenderSystem.h"

class PixelShader
{

public:
	PixelShader(const void* shader_byte_code, size_t byte_code_size, RenderSystem* system);
	~PixelShader();

private:
	ID3D11PixelShader* mPs = nullptr;
	RenderSystem* mRenderSystem = nullptr;

private:
	friend class RenderSystem;
	friend class DeviceContext;

};

