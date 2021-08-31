#pragma once

#include "Mesh.h"
#include "ResourceManager.h"

class MeshManager : public ResourceManager
{

public:
	MeshManager();
	~MeshManager();

public:
	MeshPtr createMeshFromFile(const wchar_t* file_path);
	MeshPtr createMesh(
		VertexMesh* vertex_list_data,
		unsigned int vertex_list_size,
		unsigned int* index_list_data,
		unsigned int index_list_size,
		MaterialSlot* material_slot_list,
		unsigned int material_slot_list_size
	);

protected:
	virtual Resource* createResourceFromFileConcrete(const wchar_t* file_path);

};

