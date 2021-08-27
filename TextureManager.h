#pragma once

#include "Texture.h"
#include "ResourceManager.h"

class TextureManager : public ResourceManager
{

public:
	TextureManager();
	~TextureManager();

public:
	TexturePtr createTextureFromFile(const wchar_t* file_path);
	TexturePtr createTexture(const Rect& size, Texture::Type type);

protected:
	virtual Resource* createResourceFromFileConcrete(const wchar_t* file_path);

};

