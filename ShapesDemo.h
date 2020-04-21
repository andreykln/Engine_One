#pragma once
#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"
class ShapesDemo : public Shape
{
public:
	ShapesDemo() = delete;
	ShapesDemo (ShapesDemo&) = delete;
	ShapesDemo& operator=(const ShapesDemo&) = delete;
	ShapesDemo(Graphics& gfx);
	DirectX::XMMATRIX GetTransform() const noexcept override;
// 	DirectX::XMMATRIX Update(float dt, DirectX::XMMATRIX in_matrix) noexcept override;

	DirectX::XMMATRIX* GetSphereWorldArray() const noexcept;
	DirectX::XMMATRIX* GetCylinderWorldArray() const noexcept;
	DirectX::XMMATRIX GetBoxWorld() const noexcept;
	DirectX::XMMATRIX GetGridWorld() const noexcept;
	DirectX::XMMATRIX GetCenterSphere() const noexcept;
private:
	void SetSphereWorld(DirectX::XMFLOAT4X4 source[10]);
	void SetCylinderWorld(DirectX::XMFLOAT4X4 source[10]);
	void SetBoxWorld(DirectX::XMFLOAT4X4& source);
	void SetGridWorld(DirectX::XMFLOAT4X4& source);
	void SetCenterSphere(DirectX::XMFLOAT4X4& source);

	DirectX::XMFLOAT4X4 sSphereWorld[10];
	DirectX::XMMATRIX m_SphereWorld[10];
	DirectX::XMMATRIX* pm_SphereWorld = nullptr;

	DirectX::XMFLOAT4X4 sCylWorld[10];
	DirectX::XMMATRIX m_CylWorld[10];
	DirectX::XMMATRIX* pm_CylinderWorld = nullptr;


	DirectX::XMFLOAT4X4 sBoxWorld;
	DirectX::XMMATRIX m_BoxWorld;

	DirectX::XMFLOAT4X4 sGridWorld;
	DirectX::XMMATRIX m_GridWorld;

	DirectX::XMFLOAT4X4 sCenterSphere;
	DirectX::XMMATRIX m_CenterSphere;

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

