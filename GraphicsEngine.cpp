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

	try
	{
		this->mMeshManager = new MeshManager();
	}
	catch (...) { throw std::exception("MeshManager not created successfully"); }

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

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
