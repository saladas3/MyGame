#include "GraphicsEngine.h"

// Declare the only instance used throughout the execution
GraphicsEngine* GraphicsEngine::mEngine = nullptr;

GraphicsEngine::GraphicsEngine()
{
	try
	{
		mRenderSystem = new RenderSystem();
	}
	catch (...) { throw std::exception("RenderSystem not created successfully"); }
}

GraphicsEngine::~GraphicsEngine()
{
	GraphicsEngine::mEngine = nullptr;
	delete mRenderSystem;
}

RenderSystem* GraphicsEngine::getRenderSystem()
{
	return mRenderSystem;
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
