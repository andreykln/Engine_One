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
	void SetSRVAndCBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPosition, DirectX::XMMATRIX WVP);
	int GetNumQuadFaces();
	float GetHeight(float camX, float camZ);

private:
	bool  InBounds(int i, int j);
	float Average(int i, int j);
	float GetWidth();
	float GetDepth();
	void  Smooth();
	void  CalculateAllPatchBoundsY();
	void  CalculatePatchBoundsY(UINT i, UINT j);
	void  ExtractFrustumPlanes(DirectX::XMFLOAT4 planes[6], DirectX::CXMMATRIX _M);

	CB_HS_TerrainPerFrame HullShaderCB;
	CB_VS_WorldViewProjection DomainShaderCB; 
	CB_PS_DirectionalL_Fog cbDirectionalLight;
	CB_PS_PerFrameUpdate cbPsPerFrame;
	CB_PS_Terrain terrainConstants;

	InitInfo terrainInitInfo;
	std::vector<float> heightMap;
	std::vector<DirectX::XMFLOAT2> patchBoundsY;
	const int cellsPerPatch = 64;
	int numPatchVertRows = 0;
	int numPatchVertCols = 0;
	int numPatchVertices = 0;
	int numPatchQuadFaces = 0;

	ID3D11Buffer* pHSBufferCopy = nullptr;
	ID3D11Buffer* pDSBufferCopy = nullptr;
	ID3D11Buffer* pPSBufferCopy = nullptr;
	ID3D11ShaderResourceView* pHeightMapVS = nullptr;
	ID3D11ShaderResourceView* pHeightMapDS = nullptr;
	ID3D11ShaderResourceView* pHeightMapPS = nullptr;
	ID3D11ShaderResourceView* pTerrainLayerMaps = nullptr;

	ID3D11SamplerState* pTESTSAMPLER = nullptr;

};