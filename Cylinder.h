#pragma once
#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"
class Cylinder : public Shape
{
public:
	Cylinder(Graphics& gfx, float bottom_radius, float top_radius, float height, UINT slice_count, UINT stack_count,
		bool caps, bool lightning);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	void UpdateVertexConstantBuffer(Graphics& gfx) override;
private:
	struct Vertex_C
	{
		DirectX::XMFLOAT3 pos;
		float padding0;
		DirectX::XMFLOAT3 normal;
		float padding1;
		DirectX::XMFLOAT2 tex;
	};
	GeometryGenerator::MeshData mesh;
	GeometryGenerator cylinderParts;
	CBPerFrame constLights;
	CBPerObjectTexture constMatrices;

	ID3D11Buffer* pCopyPCBLightsCylinder = nullptr;
	ID3D11Buffer* pCopyVCBMatricesCylinder = nullptr;


};

