#include "Material.h"

Material::Material(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path)
{
    void* shader_byte_code = nullptr;
    size_t size_shader = 0;

    // Create vertex shader for material
    GraphicsEngine::get()->getRenderSystem()->compileVertexShader(
        vertex_shader_path, "vsmain", &shader_byte_code, &size_shader
    );
    this->mVertexShader = GraphicsEngine::get()->getRenderSystem()->createVertexShader(
        shader_byte_code, size_shader
    );
    GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();
    if (!mVertexShader) throw std::runtime_error("Could not create vertex shader for material");

    // Create pixel shader for material
    GraphicsEngine::get()->getRenderSystem()->compilePixelShader(
        pixel_shader_path, "psmain", &shader_byte_code, &size_shader
    );
    this->mPixelShader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(
        shader_byte_code, size_shader
    );
    GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();
    if (!mPixelShader) throw std::runtime_error("Could not create pixel shader for material");
}

Material::Material(const MaterialPtr& material)
{
    this->mVertexShader = material->mVertexShader;
    this->mPixelShader = material->mPixelShader;
}

Material::~Material() = default;

void Material::addTexture(const TexturePtr& texture)
{
    this->mVecTextures.push_back(texture);
}

void Material::removeTexture(int index)
{
    if (index >= this->mVecTextures.size()) return;
    mVecTextures.erase(mVecTextures.begin() + index);
}

void Material::setData(void* data, unsigned int size)
{
    if (!mConstantBuffer)
        this->mConstantBuffer = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(
            data, size
        );
    else
        this->mConstantBuffer->update(
            GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), data
        );
}

void Material::setCullMode(CULL_MODE mode)
{
    this->mCullMode = mode;
}

CULL_MODE Material::getCullMode(CULL_MODE mode)
{
	return this->mCullMode;
}
