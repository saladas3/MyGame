#pragma once

#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "GraphicsEngine.h"

enum class CULL_MODE
{
	CULL_MODE_FRONT = 0,
	CULL_MODE_BACK
};

/// <summary>
/// The material holds more properties for a mesh, such as:
/// multiple textures,
/// point light,
/// directional light, etc.
/// </summary>
class Material
{

public:
    Material(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path);
    explicit Material(const MaterialPtr& material);
    ~Material();

public:
    void addTexture(const TexturePtr& texture);
    void removeTexture(int index);
    void setData(void* data, unsigned int size);
    void setCullMode(CULL_MODE mode);
    CULL_MODE getCullMode(CULL_MODE mode);

private:
    VertexShaderPtr mVertexShader;
    PixelShaderPtr mPixelShader;
    ConstantBufferPtr mConstantBuffer;
    std::vector<TexturePtr> mVecTextures;
    CULL_MODE mCullMode = CULL_MODE::CULL_MODE_FRONT;

private:
    friend class GraphicsEngine;

};
