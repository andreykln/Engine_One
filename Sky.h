#pragma once
#include "Shape.h"

class Sky : public Shape
{
public:
	Sky(Graphics& gfx);
	void UpdateVSMatricesAndCubeMap(Graphics& gfx, const DirectX::XMMATRIX& worldViewProj);
	void DrawSky(Graphics& gfx, const DirectX::XMMATRIX& worldViewProj);
	ID3D11ShaderResourceView** GetSkyCubeMap();

private:
	GeometryGenerator geoGen;
	GeometryGenerator::MeshData mesh;
	CB_VS_WorldViewProjection WVPmatrix;
	ID3D11Buffer* pCopyWVPBuffer = nullptr;
	ID3D11SamplerState* pSamplerState = nullptr;
	ID3D11ShaderResourceView* pCubeMapSRV = nullptr;
};