#pragma once
#include "Shape.h"

class Box : public Shape
{
public:
	Box(Graphics& gfx, float width, float height, float depth);
	ID3D11Buffer** GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	UINT GetIndexCount();

	MaterialEx boxMaterial;
	const std::wstring diffuseMap = L"bricks3";
	const std::wstring normalMap = L"bricks3_nmap";
private:
	GeometryGenerator::MeshData mesh;
	GeometryGenerator box;
	ID3D11Buffer* pVertexBuffer = nullptr;
	ID3D11Buffer* pIndexBuffer = nullptr;
	UINT indexCount = 0;
};

