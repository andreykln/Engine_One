#pragma once
#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"
class Cylinder : public Shape
{
public:
	Cylinder(Graphics& gfx, float bottom_radius, float top_radius, float height, UINT slice_count, UINT stack_count,
		DemoSwitch in_switch);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	void UpdateVertexConstantBuffer(Graphics& gfx) override;
	void UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj);

	void IncrementTexArrPos() noexcept;
	UINT GetTexArrPos() const noexcept;


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
	CBPerFrameTexArray constLightsTexArr;
	CBPerObjectTexture constMatrices;
	CB_VS_Transform transformMatrices;

	DemoSwitch demo;
	ID3D11Buffer* pCopyPCBLightsCylinder = nullptr;
	ID3D11Buffer* pCopyVCBMatricesCylinder = nullptr;

	UINT texArrPosition{};


};

