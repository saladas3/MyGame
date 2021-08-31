#include "GraphicsEngine.h"

// Declare the only instance used throughout the execution
GraphicsEngine* GraphicsEngine::mEngine = nullptr;

GraphicsEngine::GraphicsEngine()
{
	try
	{
		this->mRenderSystem = new RenderSystem();
	}
	catch (...) { throw std::exception("RenderSystem could not be created."); }

	try
	{
		this->mTexManager = new TextureManager();
	}
	catch (...) { throw std::exception("TextureManager was not created successfully"); }

	try {
		this->mMeshManager = new MeshManager();
	}
	catch (...) { throw std::exception("MeshManager not created successfully"); }

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	// Compile the vertex shader that will be responsible for computing the meshes from the '.obj' files
	this->mRenderSystem->compileVertexShader(
		L"VertexMeshLayoutShader.hlsl", "vsmain", &shader_byte_code, &size_shader
	);
	::memcpy(mMeshLayoutByteCode, shader_byte_code, size_shader);
	this->mMeshLayoutSize = size_shader;
	this->mRenderSystem->releaseCompiledShader();
}

GraphicsEngine::~GraphicsEngine()
{
	GraphicsEngine::mEngine = nullptr;
	delete mMeshManager;
	delete mTexManager;
	delete mRenderSystem;
}

RenderSystem* GraphicsEngine::getRenderSystem()
{
	return mRenderSystem;
}

TextureManager* GraphicsEngine::getTextureManager()
{
	return mTexManager;
}

MeshManager* GraphicsEngine::getMeshManager()
{
	return mMeshManager;
}

GraphicsEngine* GraphicsEngine::get()
{
	return mEngine;
}

void GraphicsEngine::create()
{
	if (GraphicsEngine::mEngine) throw std::exception("Graphics Engine already created");
	GraphicsEngine::mEngine = new GraphicsEngine();
}

void GraphicsEngine::release()
{
	if (!GraphicsEngine::mEngine) return;
	delete GraphicsEngine::mEngine;
}

MaterialPtr GraphicsEngine::createMaterial(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path)
{
	return std::make_shared<Material>(vertex_shader_path, pixel_shader_path);
}

MaterialPtr GraphicsEngine::createMaterial(const MaterialPtr& material)
{
	return std::make_shared<Material>(material);
}

void GraphicsEngine::setMaterial(const MaterialPtr& material)
{
	GraphicsEngine::get()->getRenderSystem()->setRasterizerState(
		material->mCullMode == CULL_MODE::CULL_MODE_FRONT
	);

	if (material->mConstantBuffer) {
		GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(
			material->mVertexShader, material->mConstantBuffer
		);
		GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(
			material->mPixelShader, material->mConstantBuffer
		);
	}

	// Set default shader in the graphics pipeline to be able to draw
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(material->mVertexShader);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(material->mPixelShader);

	if (material->mVecTextures.size()) {
		GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(
			material->mPixelShader,
			&material->mVecTextures[0],
			material->mVecTextures.size()
		);
	}
}

void GraphicsEngine::getVertexMeshLayoutShaderByteCodeAndSize(void** byte_code, size_t* size)
{
	*byte_code = mMeshLayoutByteCode;
	*size = mMeshLayoutSize;
}
