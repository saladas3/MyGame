#pragma once

#include "VertexMesh.h"
#include "VertexBuffer.h"
#include "GraphicsEngine.h"
#include "Vectors.h"

class Mesh : public Resource
{

public:
	Mesh(const wchar_t* full_path);
	Mesh(VertexMesh* vertex_list_data, unsigned int vertex_list_size,
		unsigned int* index_list_data, unsigned int index_list_size,
		MaterialSlot* material_slot_list, unsigned int material_slot_list_size);
	~Mesh();

public:
	const VertexBufferPtr& getVertexBuffer();
	const IndexBufferPtr& getIndexBuffer();
	const MaterialSlot& getMaterialSlot(unsigned int slot);
	size_t getNumMaterialSlots();

public:
	// Radius of the bounding sphere
	float mRadius;

private:
	// For normal mapping
	void computeTangents(
		const Vec3& v0, const Vec3& v1, const Vec3& v2,
		const Vec2& t0, const Vec2& t1, const Vec2& t2,
		Vec3& tangent, Vec3& binormal
	);
	// Calculate boundigh sphere
	float boundingSphereRadius(const std::vector<VertexMesh>& list_vertices);

private:
	VertexBufferPtr mVertexBuffer;
	IndexBufferPtr mIndexBuffer;

private:
	std::vector<MaterialSlot> mMaterialSlots;

private:
	friend class DeviceContext;

};
