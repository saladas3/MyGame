#pragma once

#include "Prerequisites.h"
#include "RenderSystem.h"

// --------------------------------------------
// Index buffer is a memory buffer that stores indices that refers to particular vertices
// Index buffer associates with the vertex buffer
// --------------------------------------------
class IndexBuffer
{

public:
	IndexBuffer(void* list_indices, UINT size_list, RenderSystem* system);
	~IndexBuffer();

public:
	[[nodiscard]] UINT getSizeIndexList() const;

private:
	UINT mSizeList = 0;
	ID3D11Buffer* mBuffer = nullptr;
	RenderSystem* mRenderSystem = nullptr;

private:
	friend class DeviceContext;

};

