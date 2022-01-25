#pragma once
#include "Shape.h"
#include "GPUWaves.h"

class WaveSurfaceGPU : public Shape
{
public:
	WaveSurfaceGPU(Graphics& gfx);
	ID3D11Buffer** GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	UINT GetIndexCount();
	cbGPUWavesVSConstData GetAndBuildConstantBufferData();

	ID3D11UnorderedAccessView* pPreviousSolutionUAV = nullptr;
	ID3D11UnorderedAccessView* pCurrentSolutionUAV = nullptr;
	ID3D11UnorderedAccessView* pNextSolutionUAV = nullptr;

	ID3D11ShaderResourceView* pPreviousSolutionSRV = nullptr;
	ID3D11ShaderResourceView* pCurrentSolutionSRV = nullptr;
	ID3D11ShaderResourceView* pNextSolutionSRV = nullptr;

	DirectX::XMMATRIX wavesWorld = DirectX::XMMatrixIdentity();
	MaterialEx wavesMat;
	const std::wstring diffuseMap = L"water2";
	//not used, but need to put something to argument
	const std::wstring normalMap = L"bricks3_nmap";
	const UINT numColumns = 256u;
	const UINT numRows = 256u;
	DirectX::XMFLOAT3 waveConstant;
private:
	ID3D11Buffer* pVertexBuffer = nullptr;
	ID3D11Buffer* pIndexBuffer = nullptr;
	UINT indexCount = 0;
	cbGPUWavesVSConstData vsConsts;
	GPUWaves wave;
	GeometryGenerator geoGen;
	GeometryGenerator::MeshData mesh;

	ID3D11Texture2D* previousSolutionTex = nullptr;
	ID3D11Texture2D* currentSolutionTex = nullptr;
	ID3D11Texture2D* nextWaveSolutionTex = nullptr;


};