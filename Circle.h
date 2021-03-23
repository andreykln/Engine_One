#pragma once
#include "Shape.h"
#include "BindableBase.h"
#include "GeometryGenerator.h"

class Circle : public Shape
{
public:
	Circle(Graphics& gfx);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	void UpdateVertexConstantBuffer(Graphics& gfx) override;
	void UpdateVSMatrices(Graphics& gfx, DirectX::XMMATRIX matrix);
	UINT GetVertices() const;
private:
// 	CBBillboardGeometry CBGeometryShader;
// 	CBBillboardPixel CBPixelShader;
// 	ID3D11Buffer* pCopyGSConstBuffer = nullptr;
// 	ID3D11Buffer* pCopyPSConstBuffer = nullptr;
// 	ID3DBlob* pVertexShaderBlob = nullptr;
	CBPerObject CBObject;
	ID3D11Buffer* pCopyVCBMatricesCircle = nullptr;
	GeometryGenerator geoGenerator;
	GeometryGenerator::MeshData mesh;
	const UINT segments = 3u;


};
