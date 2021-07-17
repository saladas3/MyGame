#pragma once

#include "Prerequisites.h"
#include "RenderSystem.h"

class VertexShader
{

public:
	VertexShader(const void* shader_byte_code, size_t byte_code_size, RenderSystem* system);
	~VertexShader();

private:
	ID3D11VertexShader* mVs = nullptr;
	RenderSystem* mRenderSystem = nullptr;

private:
	friend class RenderSystem;
	friend class DeviceContext;

};

