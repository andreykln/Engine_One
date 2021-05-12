#pragma once
#include "Shape.h"
#include "GPUWaves.h"

class WaveSurfaceGPU : public Shape
{
public:
	WaveSurfaceGPU(Graphics& gfx);
	void UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj);
	void UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPositon);
	void ClearVertexShaderResource(Graphics& gfx);
	void UpdateSolution(Graphics& gfx, float dt);
	void Disturb(Graphics& gfx);

	float t_base = 0.0f;

private:
	std::vector<Vertex_IA> vertices;
	VertexBufferDynamic* pDynamicVB = nullptr;
	ID3D11Buffer* pCopyDynamicVB = nullptr;
	CB_VS_Transform transformMatrices;
	CB_PS_DirectionalL_Fog directionalLight;
	CB_PS_PerFrameUpdate pscBuffer;
	CB_CS_GPUWaves gpuWavesCbuffer;

	GPUWaves wave;
	ID3D11Buffer* pCopyVertexConstantBuffer = nullptr;
	ID3D11Buffer* pCopyPixelConstantBuffer = nullptr;
	ID3D11Buffer* pCopyCScbuffer = nullptr;
	ID3D11SamplerState* pVSSamplerClamp = nullptr;
	ID3D11SamplerState* pVSSamplerWrap = nullptr;

	ID3D11Texture2D* pPrevSolution = nullptr;
	ID3D11Texture2D* pCurrSolution = nullptr;
	ID3D11Texture2D* pOutput = nullptr;
// 	ID3D11UnorderedAccessView* pCurrSolutionUAV = nullptr;
// 	ID3D11UnorderedAccessView* pPrevSolutionUAV = nullptr;
	ID3D11UnorderedAccessView* pOutputUAV = nullptr;

	ID3D11ShaderResourceView* pCurrSolutionSRV = nullptr;
	ID3D11ShaderResourceView* pPrevSolutionSRV = nullptr;
	ID3D11ShaderResourceView* pOutputSRV = nullptr;

	float time = 0.0f;
	const DirectX::XMMATRIX wavesScale = DirectX::XMMatrixScaling(5.0f, 5.0f, 0.0f);
	DirectX::XMFLOAT2 waterTextureOffset;
	DirectX::XMMATRIX wavesOffset;
	DirectX::XMMATRIX wavesSurfaceOffset = DirectX::XMMatrixTranslation(0.0f, -5.0f, 0.0f);
	GeometryGenerator geoGen;
	GeometryGenerator::MeshData mesh;

};