#pragma once
#include "Shape.h"

class DisplacementWaves
{
public:
	DisplacementWaves(Graphics& gfx);
	ID3D11Buffer** GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	UINT GetIndexCount();

	MaterialEx wavesMaterial;
	//water1 water2 
	const std::wstring diffuseMap = L"water1";
	const std::wstring normalMap0 = L"waves0";
	const std::wstring normalMap1 = L"waves1";
	const DirectX::XMMATRIX wavesWorld = DirectX::XMMatrixTranslation(0.0f, -1.5f, 0.0f);

private:
	GeometryGenerator geoGen;
	GeometryGenerator::MeshData mesh;
	ID3D11Buffer* pVertexBuffer = nullptr;
	ID3D11Buffer* pIndexBuffer = nullptr;
	UINT indexCount = 0;

};