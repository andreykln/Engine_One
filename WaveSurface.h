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
	void UpdateScene(float totalTime, float dt, Graphics& gfx, DirectX::XMFLOAT3& in_eyePosition);
	void UpdateVertexConstantBuffer(Graphics& gfx) override;
private:
	DirectX::XMFLOAT3 eyePosition;
	CBFog fog;
	std::vector<Vertex_IA> vertices;
	Waves wave;
	VertexBufferDynamic* pDynamicVB = nullptr;
	ID3D11Buffer* pCopyDynamicVB = nullptr;
	ID3D11Buffer* pCopyVertexConstantBuffer = nullptr;
	ID3D11Buffer* pCopyPixelConstantBuffer = nullptr;
	const DirectX::XMMATRIX wavesScale = DirectX::XMMatrixScaling(5.0f, 5.0f, 0.0f);
	DirectX::XMFLOAT2 waterTextureOffset;
	DirectX::XMMATRIX wavesOffset;
	GeometryGenerator::MeshData mesh;


};

