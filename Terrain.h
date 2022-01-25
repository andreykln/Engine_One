#pragma once
#include "Shape.h"

class Terrain
{
public:
	struct InitInfo
	{
		//RAW file
		std::wstring HeightMapFilename;
		//Scale to apply to heights after they have been loaded
		float HeightScale;
		UINT HeightMapWidth;
		UINT HeightMapHeight;
		float cellSpacing;
	};

	Terrain(Graphics& gfx);
	ID3D11Buffer** GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	UINT GetIndexCount();
	int GetNumQuadFaces();
	float GetHeight(float camX, float camZ, float camY);


	ID3D11ShaderResourceView* pLayerMaps = nullptr;
	const std::wstring snow = L"snow";
	const std::wstring blendMap = L"blend";
	const std::wstring diffuseMap = L"TerrainLayerMaps";
	const std::wstring nameHeightMap = L"TerrainHeightMap";
	const DirectX::XMMATRIX terrainWorld = DirectX::XMMatrixTranslation(-10.0f, -20.0f, 0.0f);
	ID3D11ShaderResourceView* pHeightMapVS = nullptr;
	ID3D11ShaderResourceView* pHeightMapDS = nullptr;
	ID3D11ShaderResourceView* pHeightMapPS = nullptr;
	MaterialEx terrainMaterial;
	ID3D11Buffer* pVertexBuffer = nullptr;
	ID3D11Buffer* pIndexBuffer = nullptr;
	UINT indexCount = 0;
	cbPSerrainTexel terrainConstants;
private:
	bool  InBounds(int i, int j);
	float Average(int i, int j);
	float GetWidth();
	float GetDepth();
	void  Smooth();
	void  CalculateAllPatchBoundsY();
	void  CalculatePatchBoundsY(UINT i, UINT j);

	InitInfo terrainInitInfo;
	std::vector<float> heightMap;
	std::vector<DirectX::XMFLOAT2> patchBoundsY;
	const int cellsPerPatch = 64;
	int numPatchVertRows = 0;
	int numPatchVertCols = 0;
// 	int numPatchVertices = 0;
	int numPatchQuadFaces = 0;
};