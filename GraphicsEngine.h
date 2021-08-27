#pragma once

#include "Prerequisites.h"
#include "RenderSystem.h"
#include "TextureManager.h"
#include "MeshManager.h"

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
	TextureManager* getTextureManager();
	MeshManager* getMeshManager();

public:
	// The get method that gives the only instance of GraphicsEngine
	static GraphicsEngine* get();
	static void create();
	static void release();

private:
	RenderSystem* mRenderSystem = nullptr;
	TextureManager* mTexManager = nullptr;
	MeshManager* mMeshManager = nullptr;

private:
	unsigned char mMeshLayoutByteCode[1024];
	size_t mMeshLayoutSize = 0;

private:
	static GraphicsEngine* mEngine;

};

