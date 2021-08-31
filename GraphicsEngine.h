#pragma once

#include "Prerequisites.h"
#include "RenderSystem.h"
#include "TextureManager.h"
#include "MeshManager.h"
#include "Material.h"

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

public:
	MaterialPtr createMaterial(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path);
	MaterialPtr createMaterial(const MaterialPtr& material);
	void setMaterial(const MaterialPtr& material);
	void getVertexMeshLayoutShaderByteCodeAndSize(void** byte_code, size_t* size);

private:
	RenderSystem* mRenderSystem = nullptr;
	TextureManager* mTexManager = nullptr;
	MeshManager* mMeshManager = nullptr;

private:
	unsigned char mMeshLayoutByteCode[1024];
	size_t mMeshLayoutSize = 0;

private:
	// The only instance of the graphics engine
	static GraphicsEngine* mEngine;

};

