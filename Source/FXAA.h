#pragma once
#include "Shape.h"

class FXAA
{
public:
	FXAA(Graphics& gfx);


	ID3D11ShaderResourceView* pFXAA_SRV = nullptr;
	ID3D11RenderTargetView* pFXAA_RTV = nullptr;
	ID3D11Buffer* pVertexBuffer = nullptr;
};