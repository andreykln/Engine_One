#pragma once
#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"

class GeoSphere : public Shape
{
public:
	GeoSphere(Graphics& gfx, float radius, UINT numSubdivisions);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	void UpdateVertexConstantBuffer(Graphics& gfx) override;
private:
	GeometryGenerator::MeshData mesh;
	GeometryGenerator sphere;

	CBPerFrame constLights;
	CBPerObject constMatrices;

	ID3D11Buffer* pCopyPCBLightsGeoSphere = nullptr;
	ID3D11Buffer* pCopyVCBMatricesGeoSphere = nullptr;
};

