#pragma once

#include "Resource.h"
#include "Rect.h"

class Texture : public Resource
{

public:
	enum class Type
	{
		Normal = 0,
		RenderTarget,
		DepthStencil
	};

public:
	Texture(const wchar_t* full_path);
	Texture(const Rect& size, Texture::Type type);
	~Texture();

public:
	Rect getSize();
	Texture::Type getType();

private:
	ID3D11Resource* mTexture = nullptr;
	ID3D11ShaderResourceView* mShaderResView = nullptr;
	ID3D11RenderTargetView* mRenderTargetView = nullptr;
	ID3D11DepthStencilView* mDepthStencilView = nullptr;
	ID3D11SamplerState* mSamplerState = nullptr;

	Texture::Type mType = Texture::Type::Normal;
	Rect mSize;

private:
	friend class DeviceContext;

};
