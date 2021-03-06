#pragma once
#include "Shape.h"
#include "BindableBase.h"

class DepthComplexity : public Shape 
{
public:
	DepthComplexity(Graphics& gfx);
	void UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj);
	void UpdateDepthComplexityColor(Graphics& gfx, DirectX::XMFLOAT3 color);
private:
	CBPSDepth depthColor;
	CB_VS_Transform transformMatrices;
	ID3D11Buffer* pCopyVCBMatricesDepth = nullptr;
	ID3D11Buffer* pCopyPSCBDepthColor = nullptr;
};