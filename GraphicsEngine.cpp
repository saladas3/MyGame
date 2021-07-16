#include "GraphicsEngine.h"

// Declare the only instance used throughout the execution
GraphicsEngine* GraphicsEngine::mEngine = nullptr;

GraphicsEngine::GraphicsEngine()
{
}

GraphicsEngine::~GraphicsEngine()
{
}

RenderSystem* GraphicsEngine::getRenderSystem()
{
	return mRenderSystem;
}

GraphicsEngine* GraphicsEngine::get()
{
	return mEngine;
}
