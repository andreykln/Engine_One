#pragma once

#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"
//#include "directxpackedvector.h" //for XMCOLOR
class Box : public Shape
{
public:
	Box(Graphics& gfx, float width, float height, float depth, DemoSwitch demo);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	void UpdateVertexConstantBuffer(Graphics& gfx) override;
private:
	GeometryGenerator::MeshData mesh;
	GeometryGenerator box;
	CBPerFrame constLights;
	CBPerObjectTexture constMatrices;
	DemoSwitch currentDemo;
	ID3D11Buffer* pCopyPCBLightsBox = nullptr;
	ID3D11Buffer* pCopyVCBMatricesBox = nullptr;
};

