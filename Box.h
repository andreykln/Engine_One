#pragma once

#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"
#include "directxpackedvector.h" //for XMCOLOR
class Box : public Shape
{
public:
	Box(Graphics& gfx, float width, float height, float depth);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	void UpdateVertexConstantBuffer(Graphics& gfx) override;
private:
	struct Vertex_B
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 tangent;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 tex0;
		DirectX::XMFLOAT2 tex1;
		DirectX::PackedVector::XMCOLOR color;
	};
	GeometryGenerator::MeshData mesh;
	GeometryGenerator box;
	CBPerFrame constLights;
	CBPerObject constMatrices;

private:
	ID3D11Buffer* pCopyPCBLightsBox = nullptr;
	ID3D11Buffer* pCopyVCBMatricesBox = nullptr;
};

