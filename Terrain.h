#pragma once
#include "Shape.h"
// #include "DirectXMath.h"
// #include "directxpackedvector.h"

class Terrain : public Shape
{
public:
	struct TerrainVB
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 tex;
		DirectX::XMFLOAT2 boundsY;
	};
	struct InitInfo
	{
		//RAW file
		std::wstring HeightMapFilename;
		//textures
		std::wstring LayerMapFilename0;
		std::wstring LayerMapFilename1;
		std::wstring LayerMapFilename2;
		std::wstring LayerMapFilename3;
		std::wstring LayerMapFilename4;
		std::wstring BlendMapFilename;

		//Scale to apply to heights after they have been loaded
		float HeightScale;
		UINT HeightMapWidth;
		UINT HeightMapHeight;
		float cellSpacing;
	};

	Terrain(Graphics& gfx);
	void SetSRVAndCBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPosition, DirectX::XMFLOAT4 worldFrustumPlanes[6],
		DirectX::XMMATRIX WVP);

private:
	bool InBounds(int i, int j);
	float Average(int i, int j);
	float GetWidth();
	float GetDepth();
	void Smooth();
	void CalculateAllPatchBoundsY();
	void CalculatePatchBoundsY(UINT i, UINT j);

	CB_HS_TerrainPerFrame HullShaderCB;
	CB_VS_WorldViewProjection DomainShaderCB; 


	InitInfo terrainInitInfo;
	std::vector<float> heightMap;
	std::vector<DirectX::XMFLOAT2> patchBoundsY;
	ID3D11ShaderResourceView* pHeightMapSRV = nullptr;
	const int cellsPerPatch = 64;
	int numPatchVertRows = 0;
	int numPatchVertCols = 0;
	int numPatchVertices = 0;
	int numPatchQuadFaces = 0;

	ID3D11Buffer* pHSBufferCopy = nullptr;
	ID3D11Buffer* pDSBufferCopy = nullptr;
	ID3D11ShaderResourceView* pTerrainLayerMaps = nullptr;
};