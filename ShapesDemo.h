#pragma once
#include "Shape.h"
#include <array>
class ShapesDemo 
{
public:
	ShapesDemo (ShapesDemo&) = delete;
	ShapesDemo& operator=(const ShapesDemo&) = delete;
	ShapesDemo();
	DirectX::XMMATRIX*& GetSphereWorldArray() noexcept;
	DirectX::XMMATRIX*& GetCylinderWorldArray()noexcept;
	DirectX::XMMATRIX Get_m_BoxWorld() const noexcept;
	DirectX::XMMATRIX Get_m_GridWorld() const noexcept;
	DirectX::XMMATRIX Get_m_CenterSphere() const noexcept;
	DirectX::XMMATRIX GetCameraOffset() const noexcept;
private:
	void SetSphereWorld(DirectX::XMFLOAT4X4 source[10]);
	void SetCylinderWorld(DirectX::XMFLOAT4X4 source[10]);
	void SetBoxWorld(DirectX::XMFLOAT4X4& source);
	void SetGridWorld(DirectX::XMFLOAT4X4& source);
	void SetCenterSphere(DirectX::XMFLOAT4X4& source);

	const DirectX::XMMATRIX cameraOffset = 
			DirectX::XMMatrixTranslation(0.0f, -4.0f, 0.0f);

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
};

