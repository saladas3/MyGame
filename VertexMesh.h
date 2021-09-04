#pragma once

#include "Vectors.h"

struct MaterialSlot
{
	size_t StartIndex = 0;
	size_t NumIndices = 0;
	size_t MaterialId = 0;
};

class VertexMesh
{

public:
	VertexMesh() : mPosition(), mTexcoord(), mNormal() {	}
	VertexMesh(
		const Vec3& position,
		const Vec2& texcoord,
		const Vec3& normal,
		const Vec3& tangent,
		const Vec3& binormal) :
		mPosition(position),
		mTexcoord(texcoord),
		mNormal(normal),
		mTangent(tangent),
		mBinormal(binormal) { }
	VertexMesh(const VertexMesh& vertex) :
		mPosition(vertex.mPosition),
		mTexcoord(vertex.mTexcoord),
		mNormal(vertex.mNormal),
		mTangent(vertex.mTangent),
		mBinormal(vertex.mBinormal) { }
	~VertexMesh() = default;

public:
	Vec3 mPosition;
	Vec2 mTexcoord;
	Vec3 mNormal;
	Vec3 mTangent;
	Vec3 mBinormal;

};
