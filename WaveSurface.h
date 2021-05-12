#pragma once
#include "Shape.h"
#include "Waves.h"
#include "MathHelper.h"
#include "GeometryGenerator.h"


class WaveSurface : public Shape
{
public:
	WaveSurface(Graphics& gfx);
	void UpdateScene(float totalTime, float dt, Graphics& gfx);
	void UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj);
	void UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPositon);
	DirectX::XMMATRIX GetWaveSurfaceOffset();
private:
	DirectX::XMFLOAT3 eyePosition;

	std::vector<Vertex_IA> vertices;
	Waves wave;
	VertexBufferDynamic* pDynamicVB = nullptr;
	ID3D11Buffer* pCopyDynamicVB = nullptr;
	CB_VS_Transform transformMatrices;
	CB_PS_DirectionalL_Fog directionalLight;
	CB_PS_PerFrameUpdate pscBuffer;


	ID3D11Buffer* pCopyVertexConstantBuffer = nullptr;
	ID3D11Buffer* pCopyPixelConstantBuffer = nullptr;
	const DirectX::XMMATRIX wavesScale = DirectX::XMMatrixScaling(5.0f, 5.0f, 0.0f);
	DirectX::XMFLOAT2 waterTextureOffset;
	DirectX::XMMATRIX wavesOffset;
	DirectX::XMMATRIX wavesSurfaceOffset = DirectX::XMMatrixTranslation(0.0f, -5.0f, 0.0f);
	GeometryGenerator::MeshData mesh;


};

