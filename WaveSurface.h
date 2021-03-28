#pragma once
#include "Shape.h"
#include "Waves.h"
#include "BindableBase.h"
#include "MathHelper.h"
#include "GeometryGenerator.h"


class WaveSurface : public Shape
{
public:
	CBPerObjectTexture perObjectMatrices;
	CBPerFrame perFrameLight;
	WaveSurface(Graphics& gfx);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	void UpdateScene(float totalTime, float dt, Graphics& gfx);
	void UpdateVertexConstantBuffer(Graphics& gfx) override;
	void UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj);

private:
	DirectX::XMFLOAT3 eyePosition;

	std::vector<Vertex_IA> vertices;
	Waves wave;
	VertexBufferDynamic* pDynamicVB = nullptr;
	ID3D11Buffer* pCopyDynamicVB = nullptr;
	CB_VS_Transform transformMatrices;
	
	ID3D11Buffer* pCopyVertexConstantBuffer = nullptr;
	ID3D11Buffer* pCopyPixelConstantBuffer = nullptr;
	const DirectX::XMMATRIX wavesScale = DirectX::XMMatrixScaling(5.0f, 5.0f, 0.0f);
	DirectX::XMFLOAT2 waterTextureOffset;
	DirectX::XMMATRIX wavesOffset;
	GeometryGenerator::MeshData mesh;


};

