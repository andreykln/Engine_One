#pragma once
#include "Shape.h"

class GeoSphere
{
public:
	GeoSphere(Graphics& gfx, float radius, UINT numSubdivisions);
	ID3D11Buffer** GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	UINT GetIndexCount();

	MaterialEx sphereMaterial;
	//anything for those two, they are not used for spheres in main demo
	const std::wstring diffuseMap = L"bricks3";
	const std::wstring normalMap = L"bricks3_nmap";
	DirectX::XMMATRIX m_SphereWorld[10];
private:
	GeometryGenerator::MeshData mesh;
	GeometryGenerator sphere;
	DirectX::XMFLOAT4X4 sGeoSphereWorld[10];
	ID3D11Buffer* pVertexBuffer = nullptr;
	ID3D11Buffer* pIndexBuffer = nullptr;
	UINT indexCount = 0;
};

