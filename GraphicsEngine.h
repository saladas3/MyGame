#pragma once

#include "Prerequisites.h"
#include "RenderSystem.h"

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
	static void create();
	static void release();

private:
	RenderSystem* mRenderSystem = nullptr;

private:
	unsigned char mMeshLayoutByteCode[1024];
	size_t mMeshLayoutSize = 0;

private:
	static GraphicsEngine* mEngine;

};

