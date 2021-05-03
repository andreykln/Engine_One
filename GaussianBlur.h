#pragma once
#include "Shape.h"
#include "DrawableBase.h"
#include "GeometryGenerator.h"

class GaussianBlur : public Shape 
{
public:
	GaussianBlur(Graphics& gfx);
	ID3D11RenderTargetView* GetRTV() const;
	void PerformBlur(Graphics& gfx);
	ID3D11Texture2D* GetBlurredtexture();
	void UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj);

private:
	GeometryGenerator::MeshData mesh;
	GeometryGenerator geoGen;
	CB_VS_Transform transformMatrices;
	CB_PS_DirectionalL_Fog directionalLight;
	CB_PS_PerFrameUpdate pscBuffer;

// 	ID3D11Buffer* pCopyConstVertBufferBlur = nullptr;
// 	ID3D11Buffer* pCopyBlurPSBuffer = nullptr;

	ID3D11RenderTargetView* pRTVtoBlur = nullptr;
	ID3D11Texture2D* pTextureToBlur = nullptr;
	ID3D11UnorderedAccessView* pUAV = nullptr;
	ID3D11ShaderResourceView* pSRV = nullptr;

// 	ID3D11Texture2D* pBlurredOutputTex = nullptr;
	ID3D11UnorderedAccessView* pBlurredOutputUAV = nullptr;
	ID3D11ShaderResourceView* pBlurredOutputSRV = nullptr;


	ID3D11Buffer* pCopyVCBBlur = nullptr;
};
