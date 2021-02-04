#pragma once
#include "Shape.h"
#include "BindableBase.h"

class DepthComplexity : public Shape 
{
public:
	DepthComplexity(Graphics& gfx);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	void UpdateVertexConstantBuffer(Graphics& gfx) override;

private:
	CBPerObject matrices;
	ID3D11Buffer* pCopyVCBMatricesDepth;
// 	DirectX::XMMATRIX m_Matrix = DirectX::XMMatrixIdentity();
// 	DirectX::XMMATRIX m_Centered = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
};