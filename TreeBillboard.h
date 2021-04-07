#pragma once
#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"


class TreeBillboard : public Shape 
{
public:
	TreeBillboard(Graphics& gfx);
	void UpdateConstantBuffers(Graphics& gfx, DirectX::XMFLOAT3& eyePosition);
private:
	void UpdateGeometryConstBufer(Graphics& gfx, DirectX::XMFLOAT3& eyePosition);
	void UpdatePixelShaderConstBuffer(Graphics& gfx);
	CBBillboardGeometry CBGeometryShader;
	CBBillboardPixel CBPixelShader;
	ID3D11Buffer* pCopyGSConstBuffer = nullptr;
	ID3D11Buffer* pCopyPSConstBuffer = nullptr;
	ID3DBlob* pVertexShaderBlob = nullptr;
	DirectionalLight dirLight_[3];


};