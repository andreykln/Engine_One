#pragma once
#include "Shape.h"

class QuadTessellation : public Shape
{
public:
	QuadTessellation(Graphics& gfx);
	void UpdateTessellationShaderBuffers(Graphics& gfx, DirectX::XMMATRIX WVP, DirectX::XMMATRIX world, DirectX::XMFLOAT3 cameraPos);
private:
	CB_QuadTess_DS_WVP cbDSworldViewProj;
	CB_QuadTess_HS cbHullShader;

	ID3D11Buffer* pCopyCBHS = nullptr;
	ID3D11Buffer* pCopyCBDS = nullptr;

};