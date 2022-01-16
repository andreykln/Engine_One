#pragma once
#include "Shape.h"

class Cylinder : public Shape
{
public:
	Cylinder(Graphics& gfx, float bottom_radius, float top_radius, float height, UINT slice_count, UINT stack_count);
	ID3D11Buffer** GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	UINT GetIndexCount();

	MaterialEx cylinderMaterial;
	const std::wstring diffuseMap = L"bricks3";
	const std::wstring normalMap = L"bricks3_nmap";
	DirectX::XMMATRIX m_CylWorld[10];
private:
	GeometryGenerator::MeshData mesh;
	GeometryGenerator cylinderParts;
	DirectX::XMFLOAT4X4 sCylWorld[10];
	ID3D11Buffer* pVertexBuffer = nullptr;
	ID3D11Buffer* pIndexBuffer = nullptr;
	UINT indexCount = 0;
};

