#pragma once
#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"
class ShapesDemo : public Shape
{
public:
	ShapesDemo(Graphics& gfx);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	DirectX::XMMATRIX Update(float dt, DirectX::XMMATRIX in_matrix) noexcept override;

	void GetSphereWorld(DirectX::XMFLOAT4X4 source[10]);
	DirectX::XMMATRIX GetCylinderWorld(DirectX::XMFLOAT4X4& source);
	DirectX::XMMATRIX GetBoxWorld(DirectX::XMFLOAT4X4& source);
	DirectX::XMMATRIX GetGridWorld(DirectX::XMFLOAT4X4& source);
	DirectX::XMMATRIX GetCenterSphere(DirectX::XMFLOAT4X4& source);
private:
	DirectX::XMFLOAT4X4 sSphereWorld[10];
	DirectX::XMMATRIX m_SphereWorld[10];
	DirectX::XMFLOAT4X4 sCylWorld[10];
	DirectX::XMFLOAT4X4 sBoxWorld;
	DirectX::XMFLOAT4X4 sGridWorld;
	DirectX::XMFLOAT4X4 sCenterSphere;

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

