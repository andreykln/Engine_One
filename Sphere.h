#pragma once
#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"

class Sphere : public Shape
{
public:
	Sphere(Graphics& gfx, float radius, UINT sliceCount, UINT stackCount);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	void UpdateVertexConstantBuffer(Graphics& gfx) override;
private:
	GeometryGenerator::MeshData mesh;
	GeometryGenerator sphere;
	CBPerFrame constLights;
	CBPerObject constMatrices;
	ID3D11Buffer* pCopyPCBLightsSphere = nullptr;
	ID3D11Buffer* pCopyVCBMatricesSphere = nullptr;

};

