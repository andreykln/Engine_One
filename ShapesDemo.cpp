#include "ShapesDemo.h"

ShapesDemo::ShapesDemo()
{
	DirectX::XMMATRIX I = DirectX::XMMatrixIdentity();

	DirectX::XMStoreFloat4x4(&sGridWorld, I);
	DirectX::XMMATRIX boxScale = DirectX::XMMatrixScaling(2.0f, 1.0f, 2.0f);
	DirectX::XMMATRIX boxOffset = DirectX::XMMatrixTranslation(0.0f, 0.5f, 0.0f); //TODO z might be 4.0f
	DirectX::XMStoreFloat4x4(&sBoxWorld, DirectX::XMMatrixMultiply(boxScale, boxOffset));

	DirectX::XMMATRIX centerSphereScale = DirectX::XMMatrixScaling(2.0f, 2.0f, 2.0f);
	DirectX::XMMATRIX centerSphereOffset = DirectX::XMMatrixTranslation(0.0f, 2.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&sCenterSphere, DirectX::XMMatrixMultiply(centerSphereScale, centerSphereOffset));

	//5 rows of 2 cylinders and spheres per row
	for (int i = 0; i < 5; ++i)
	{
		DirectX::XMStoreFloat4x4(&sCylWorld[i * 2 + 0], //for showing that we add row.
			DirectX::XMMatrixTranslation(-5.0f, 1.5f, -10.0f + i * 5.0f));

		DirectX::XMStoreFloat4x4(&sCylWorld[i * 2 + 1],
			DirectX::XMMatrixTranslation(5.0f, 1.5f, -10.0f + i * 5.0f));

		DirectX::XMStoreFloat4x4(&sSphereWorld[i * 2 + 0],
			DirectX::XMMatrixTranslation(-5.0f, 3.5f, -10.0f + i * 5.0f));

		DirectX::XMStoreFloat4x4(&sSphereWorld[i * 2 + 1],
			DirectX::XMMatrixTranslation(5.0f, 3.5f, -10.0f + i * 5.0f));
	}

	SetSphereWorld(sSphereWorld);
	SetCylinderWorld(sCylWorld);
	SetBoxWorld(sBoxWorld);
	SetGridWorld(sGridWorld);
	SetCenterSphere(sCenterSphere);
}

DirectX::XMMATRIX*& ShapesDemo::GetSphereWorldArray() noexcept
{
	return pm_SphereWorld;
}

//return ref to pointer for pointer arithmetics
DirectX::XMMATRIX*& ShapesDemo::GetCylinderWorldArray() noexcept
{
	return pm_CylinderWorld;
}

DirectX::XMMATRIX ShapesDemo::Get_m_BoxWorld() const noexcept
{
	return m_BoxWorld;
}

DirectX::XMMATRIX ShapesDemo::Get_m_GridWorld() const noexcept
{
	return m_GridWorld;
}

DirectX::XMMATRIX ShapesDemo::Get_m_CenterSphere() const noexcept
{
	return m_CenterSphere;
}

DirectX::XMMATRIX ShapesDemo::GetCameraOffset() const noexcept
{
	return cameraOffset;
}

void ShapesDemo::SetSphereWorld(DirectX::XMFLOAT4X4 source[10])
{
	for (size_t i = 0; i < 10; i++)
	{
		m_SphereWorld[i] = DirectX::XMLoadFloat4x4(&source[i]);
	}
	pm_SphereWorld = m_SphereWorld;
}

void ShapesDemo::SetCylinderWorld(DirectX::XMFLOAT4X4 source[10])
{
	for (size_t i = 0; i < 10; i++)
	{
		m_CylWorld[i] = DirectX::XMLoadFloat4x4(&source[i]);
	}
	pm_CylinderWorld = m_CylWorld;
}

void ShapesDemo::SetBoxWorld(DirectX::XMFLOAT4X4& source)
{
	m_BoxWorld = DirectX::XMLoadFloat4x4(&source);
}

void ShapesDemo::SetGridWorld(DirectX::XMFLOAT4X4& source)
{
	m_GridWorld = DirectX::XMLoadFloat4x4(&source);
}

void ShapesDemo::SetCenterSphere(DirectX::XMFLOAT4X4& source)
{
	m_CenterSphere = DirectX::XMLoadFloat4x4(&source);
}
