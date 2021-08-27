#include "TextureManager.h"

TextureManager::TextureManager() : ResourceManager() {};

TextureManager::~TextureManager() = default;

TexturePtr TextureManager::createTextureFromFile(const wchar_t* file_path)
{
	return std::static_pointer_cast<Texture>(createResourceFromFile(file_path));
}

TexturePtr TextureManager::createTexture(const Rect& size, Texture::Type type)
{
	return TexturePtr(new Texture(size, type));
}

Resource* TextureManager::createResourceFromFileConcrete(const wchar_t* file_path)
{
	return new Texture(file_path);
}
