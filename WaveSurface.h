#pragma once
#include "Shape.h"
#include "Waves.h"
#include "BindableBase.h"
#include "MathHelper.h"
#include "GeometryGenerator.h"


class WaveSurface : public Shape
{
public:
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		float padding0 = 0.0f;
		DirectX::XMFLOAT3 normal;
		float padding1 = 0.0f;
		DirectX::XMFLOAT2 tex;
	
	};
	CBPerObjectTexture perObjectMatrices;
	CBPerFrame perFrameLight;
	WaveSurface(Graphics& gfx);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	void UpdateScene(float totalTime, float dt, Graphics& gfx);
	void UpdateVertexConstantBuffer(Graphics& gfx) override;
public:
//	float alpha{};
private:
	std::vector<Vertex> vertices;
	Waves wave;
	VertexBufferDynamic* pDynamicVB = nullptr;
	ID3D11Buffer* pCopyDynamicVB = nullptr;
	ID3D11Buffer* pCopyVertexConstantBuffer = nullptr;
	ID3D11Buffer* pCopyPixelConstantBuffer = nullptr;
	const DirectX::XMMATRIX wavesScale = DirectX::XMMatrixScaling(5.0f, 5.0f, 0.0f);
	DirectX::XMFLOAT2 waterTextureOffset;
	DirectX::XMMATRIX wavesOffset;


	GeometryGenerator::MeshData mesh;
	//GeometryGenerator sphere;

};

