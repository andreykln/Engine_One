#pragma once
#include "DrawableBase.h"

class SSAO : public Shape
{
public:
	SSAO(Graphics& gfx, UINT mWidth, UINT mHeight);
	ID3D11RenderTargetView* GetSSAO_RTV();
	void SetNormalDepthRenderTarget(Graphics& gfx, ID3D11DepthStencilView* dsv);
private:
	ID3D11ShaderResourceView* pNormalMapSRV = nullptr;
	ID3D11RenderTargetView* pRTV = nullptr;

};
