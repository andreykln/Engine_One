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





	void UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj, float dt);
	void UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPositon);
	void ClearVertexShaderResource(Graphics& gfx);
	void UpdateSolution(Graphics& gfx, float dt);
	void Disturb(Graphics& gfx);



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
	//////////////////////////////////////////////////////////////////////////
	ID3D11Buffer* pVertexBuffer = nullptr;
	ID3D11Buffer* pIndexBuffer = nullptr;
	UINT indexCount = 0;
	cbGPUWavesVSConstData vsConsts;
	GPUWaves wave;
	//////////////////////////////////////////////////////////////////////////



// 	std::vector<Vertex_IA> vertices;
	ID3D11Buffer* pCopyDynamicVB = nullptr;
	CB_VS_Transform transformMatrices;
	CB_PS_DirectionalL_Fog directionalLight;
	CB_PS_PerFrameUpdate pscBuffer;
// 	CB_CS_GPUWaves gpuWavesCbuffer;

	ID3D11Buffer* pCopyVertexConstantBuffer = nullptr;
	ID3D11Buffer* pCopyPixelConstantBuffer = nullptr;
	ID3D11Buffer* pCopyCScbuffer = nullptr;
	ID3D11SamplerState* pVSSamplerClamp = nullptr;
	ID3D11SamplerState* pVSSamplerWrap = nullptr;

	ID3D11Texture2D* previousSolutionTex = nullptr;
	ID3D11Texture2D* currentSolutionTex = nullptr;
	ID3D11Texture2D* nextWaveSolutionTex = nullptr;

	float time = 0.0f;
	const DirectX::XMMATRIX wavesScale = DirectX::XMMatrixScaling(5.0f, 5.0f, 1.0f);
	DirectX::XMFLOAT2 waterTextureOffset;
	DirectX::XMMATRIX wavesOffset;
	DirectX::XMMATRIX wavesSurfaceOffset = DirectX::XMMatrixTranslation(0.0f, -5.0f, 0.0f);
	GeometryGenerator geoGen;
	GeometryGenerator::MeshData mesh;


};