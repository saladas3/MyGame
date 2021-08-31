#include "Texture.h"
#include "GraphicsEngine.h"

Texture::Texture(const wchar_t* full_path) : Resource(full_path)
{
	DirectX::ScratchImage image_data;
	HRESULT res = DirectX::LoadFromWICFile(
		full_path, DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, image_data
	);

	if (SUCCEEDED(res))
	{
		res = DirectX::CreateTexture(
			GraphicsEngine::get()->getRenderSystem()->mD3DDevice, 
			image_data.GetImages(), 
			image_data.GetImageCount(), 
			image_data.GetMetadata(), 
			&mTexture
		);

		if (FAILED(res)) throw std::exception("Could not create Texture");

		D3D11_SHADER_RESOURCE_VIEW_DESC shader_res_view_desc = {};
		shader_res_view_desc.Format = image_data.GetMetadata().format;
		shader_res_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shader_res_view_desc.Texture2D.MipLevels = image_data.GetMetadata().mipLevels;
		shader_res_view_desc.Texture2D.MostDetailedMip = 0;

		D3D11_SAMPLER_DESC sampler_desc = {};
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = (UINT)image_data.GetMetadata().mipLevels;

		res = GraphicsEngine::get()->getRenderSystem()->mD3DDevice->CreateSamplerState(
			&sampler_desc, &mSamplerState
		);
		if (FAILED(res)) throw std::exception("Could not create sampler state for texture");

		res = GraphicsEngine::get()->getRenderSystem()->mD3DDevice->CreateShaderResourceView(
			mTexture, &shader_res_view_desc, &mShaderResView
		);
		if (FAILED(res)) throw std::exception("Could not create shader resource view for texture");
	}
	else
	{
		throw std::exception("Could not read texture from file");
	}
}

Texture::Texture(const Rect& size, Texture::Type type) : Resource(L"")
{
	D3D11_TEXTURE2D_DESC tex_desc = {};
	tex_desc.Width = size.width;
	tex_desc.Height = size.height;

	if (type == Texture::Type::Normal) {
		tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	}
	else if (type == Texture::Type::RenderTarget) {
		tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		tex_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	}
	else if (type == Texture::Type::DepthStencil) {
		tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	}

	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.MipLevels = 1;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.MiscFlags = 0;
	tex_desc.ArraySize = 1;
	tex_desc.CPUAccessFlags = 0;

	auto hr = GraphicsEngine::get()->getRenderSystem()->mD3DDevice->CreateTexture2D(
		&tex_desc, nullptr, (ID3D11Texture2D**)&mTexture
	);
	if (FAILED(hr)) throw std::exception("Texture was not created successfully");

	if (type == Texture::Type::Normal || type == Texture::Type::RenderTarget) {
		hr = GraphicsEngine::get()->getRenderSystem()->mD3DDevice->CreateShaderResourceView(
			mTexture, NULL, &mShaderResView
		);
		if (FAILED(hr)) throw std::exception("Texture was not created successfully");
	}

	if (type == Texture::Type::RenderTarget) {
		hr = GraphicsEngine::get()->getRenderSystem()->mD3DDevice->CreateRenderTargetView(
			mTexture, NULL, &mRenderTargetView
		);
		if (FAILED(hr)) throw std::exception("Texture was not created successfully");
	}

	else if (type == Texture::Type::DepthStencil) {
		hr = GraphicsEngine::get()->getRenderSystem()->mD3DDevice->CreateDepthStencilView(
			mTexture, NULL, &mDepthStencilView
		);
		if (FAILED(hr)) throw std::exception("Texture was not created successfully");
	}

	mType = type;
	mSize = size;
}

Texture::~Texture()
{
	if (mShaderResView) mShaderResView->Release();
	if (mRenderTargetView) mRenderTargetView->Release();
	if (mDepthStencilView) mDepthStencilView->Release();
	if (mSamplerState) mSamplerState->Release();
	if (mTexture) mTexture->Release();
}

Rect Texture::getSize()
{
	return mSize;
}

Texture::Type Texture::getType()
{
	return mType;
}
