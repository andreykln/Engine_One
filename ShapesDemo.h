#pragma once
#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"
class ShapesDemo : public Shape
{
public:
	ShapesDemo(Graphics& gfx);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
private:
	GeometryGenerator::MeshData box;
	GeometryGenerator::MeshData grid;
	GeometryGenerator::MeshData sphere;
	GeometryGenerator::MeshData cylinder;
	GeometryGenerator generator;
	UINT mBoxVertexOffset{};
	UINT mGridVertexOffset{};
	UINT mSphereVertexOffset{};
	UINT mCylinderVertexOffset{};

	UINT mBoxIndexOffset{};
	UINT mGridIndexOffset{};
	UINT mSphereIndexOffset{};
	UINT mCylinderIndexOffset{};

	UINT mBoxIndexCount{};
	UINT mGridIndexCount{};
	UINT mSphereIndexCount{};
	UINT mCylinderIndexCount{};
};

