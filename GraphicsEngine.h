#pragma once

#include "Prerequisites.h"

class GraphicsEngine
{

// Graphics engine class will be a singleton
private:
	// Init the Graphics Engine and DirectX 11 Device
	GraphicsEngine();
	// Release all the resources loaded
	~GraphicsEngine();

public:
	RenderSystem* getRenderSystem();

public:
	// The get method that gives the only instance of GraphicsEngine
	static GraphicsEngine* get();

private:
	RenderSystem* mRenderSystem = nullptr;

private:
	static GraphicsEngine* mEngine;

};

