#pragma once
#include "Shape.h"
class SSAO
{
public:
	SSAO(Graphics& gfx, UINT mWidth, UINT mHeight);
	ID3D11Buffer** GetQuadVertexBuffer();
	ID3D11Buffer* GetQuadIndexBuffer();
	UINT GetQuadIndexCount();
	cbComputeSSAOconstData GetAndBuildConstantBufferData();
	cbComputeSSAO& GetConstBufferSSAODataPerFrame();
	ID3D11ShaderResourceView* GetRandomVectorSRV();
	D3D11_VIEWPORT& GetSSAOViewport();

	ID3D11RenderTargetView* GetAmbientMapRTV0();
	ID3D11RenderTargetView* GetAmbientMapRTV1();
	ID3D11ShaderResourceView* GetAmbientMapSRV0();
	ID3D11ShaderResourceView* GetAmbientMapSRV1();
	void SetNormalDepthRenderTarget(Graphics& gfx, ID3D11DepthStencilView* dsv);
	ID3D11ShaderResourceView* GetNormalMapSRV();

private:
	void BuildRandomVectorTexture(Graphics& gfx);
	void BuildTextureViewsAndViewport(Graphics& gfx, UINT mWidth, UINT mHeight);
	void BuildFrustumFarCorners(float fovy, float farZ);
	void BuildOffsetVectors();
	void BuildFullScreenQuadBuffers(Graphics& gfx);

	ID3D11ShaderResourceView* pNormalMapSRV = nullptr;
	ID3D11ShaderResourceView* pRandomVectorsSRV = nullptr;
	ID3D11RenderTargetView* pNormalMapRTV = nullptr;
	//two for blur
	ID3D11RenderTargetView* pAmbientRTV0 = nullptr;
	ID3D11RenderTargetView* pAmbientRTV1 = nullptr;
	ID3D11ShaderResourceView* pAmbientSRV0 = nullptr;
	ID3D11ShaderResourceView* pAmbientSRV1 = nullptr;

	ID3D11Buffer* pSSAOConstBuffer = nullptr;
	ID3D11Buffer* pSSAOBlurBuffer = nullptr;
	ID3D11Buffer* pQuadVertexBuffer = nullptr;
	ID3D11Buffer* pQuadIndexBuffer = nullptr;
	UINT quadIndexCount = 0;
	D3D11_VIEWPORT vp;
	cbComputeSSAO computeSSAOcbuff;
	cbComputeSSAOconstData computeSSAOData;

	DirectX::XMFLOAT4 frustumFarCorner[4];
	DirectX::XMFLOAT4 offsets[14];
	DirectX::XMMATRIX viewToTextureSpace;
};