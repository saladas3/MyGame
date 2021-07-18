#pragma once

#include "Prerequisites.h"
#include "RenderSystem.h"

/*
* Holds information about the vertices of the mesh.
*/

class VertexBuffer
{

public:
	VertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, 
		void* shader_byte_code, size_t size_byte_shader, RenderSystem* system);
	~VertexBuffer();

public:
	UINT getSizeVertexList();

private:
	UINT mSizeVertex = 0;
	UINT mSizeList = 0;
	ID3D11Buffer* mBuffer = nullptr;
	ID3D11InputLayout* mLayout = nullptr;
	RenderSystem* mRenderSystem = nullptr;

private:
	friend class DeviceContext;

};

