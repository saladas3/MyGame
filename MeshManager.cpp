#include "MeshManager.h"

MeshManager::MeshManager() : ResourceManager() { }

MeshManager::~MeshManager() = default;

MeshPtr MeshManager::createMeshFromFile(const wchar_t* file_path)
{
	return std::static_pointer_cast<Mesh>(createResourceFromFile(file_path));
}

MeshPtr MeshManager::createMesh(
	VertexMesh* vertex_list_data,
	unsigned int vertex_list_size,
	unsigned int* index_list_data,
	unsigned int index_list_size,
	MaterialSlot* material_slot_list,
	unsigned int material_slot_list_size
)
{
	return MeshPtr(new Mesh(
		vertex_list_data,
		vertex_list_size,
		index_list_data,
		index_list_size,
		material_slot_list,
		material_slot_list_size)
	);
}

Resource* MeshManager::createResourceFromFileConcrete(const wchar_t* file_path)
{
	return new Mesh(file_path);
}
