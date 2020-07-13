#pragma once

#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"
//#include "directxpackedvector.h" //for XMCOLOR
class Box : public Shape
{
public:
	Box(Graphics& gfx, float width, float height, float depth, bool isDemo);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	void UpdateVertexConstantBuffer(Graphics& gfx) override;
private:
	struct Vertex_B
	{
		DirectX::XMFLOAT3 pos;
// 		DirectX::XMFLOAT3 tangent;
		DirectX::XMFLOAT2 tex0;

		DirectX::XMFLOAT3 normal;
// 		DirectX::XMFLOAT2 tex1;
	};
// 	struct CBPerObjectTexture
// 	{
// 		CBPerObjectTexture() { ZeroMemory(this, sizeof(this)); }
// 		DirectX::XMMATRIX gWorld;
// 		DirectX::XMMATRIX gWorldInvTranspose;
// 		DirectX::XMMATRIX gWorldViewProj;
// 		DirectX::XMMATRIX gTexTransform;
// 	};

	GeometryGenerator::MeshData mesh;
	GeometryGenerator box;
	CBPerFrame constLights;
	CBPerObjectTexture constMatrices;

private:
	bool shapesDemo = false;
	ID3D11Buffer* pCopyPCBLightsBox = nullptr;
	ID3D11Buffer* pCopyVCBMatricesBox = nullptr;
};

