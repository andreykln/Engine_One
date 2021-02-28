#pragma once
#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"


class TreeBillboard : public Shape 
{
public:
	TreeBillboard(Graphics& gfx, std::vector<TreePointSprite> in_trees_pos);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	void UpdateVertexConstantBuffer(Graphics& gfx) override;
	void UpdateConstantBuffers(Graphics& gfx, DirectX::XMFLOAT3& eyePosition);
private:
	void UpdateGeometryConstBufer(Graphics& gfx, DirectX::XMFLOAT3& eyePosition);
	void UpdatePixelShaderConstBuffer(Graphics& gfx);
	CBBillboardGeometry CBGeometryShader;
	CBBillboardPixel CBPixelShader;
	ID3D11Buffer* pCopyGSConstBuffer = nullptr;
	ID3D11Buffer* pCopyPSConstBuffer = nullptr;
	std::vector<TreePointSprite> treePositions;
	ID3DBlob* pVertexShaderBlob = nullptr;
	DirectionalLight dirLight_[3];


};