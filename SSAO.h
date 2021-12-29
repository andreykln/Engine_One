#pragma once
#include "DrawableBase.h"
#include "directxpackedvector.h"
class SSAO : public Shape
{
public:
	SSAO(Graphics& gfx, UINT mWidth, UINT mHeight);
	ID3D11ShaderResourceView* GetNormalMapSRV();
	ID3D11ShaderResourceView* GetRandomVectorSRV();
	void ComputeSSAO(Graphics& gfx, DirectX::XMMATRIX mProj);
	void SetNormalDepthRenderTarget(Graphics& gfx, ID3D11DepthStencilView* dsv);
private:
	struct SSAOConstBuffer
	{
		DirectX::XMMATRIX viewToTexSpace;
		DirectX::XMVECTOR offsetVectors[14];
		DirectX::XMFLOAT4 frustumFarCorners[4];

		const float gOcclusionRadius = 0.5f;
		const float gOcclusionFadeStart = 0.2f;
		const float gOcclusionFadeEnd = 2.0f;
		const float gSurfaceEpsilon = 0.05f;
	};

	void BuildRandomVectorTexture(Graphics& gfx);
	void BuildConstantBuffer(Graphics& gfx);
	void BuildTextureViewsAndViewport(Graphics& gfx, UINT mWidth, UINT mHeight);
	void BuildFrustumFarCorners(float fovy, float farZ);
	void BuildOffsetVectors();
	void BuildFullScreenQuadBuffers(Graphics& gfx);
	void BuildSamplers(Graphics& gfx);

	void UpdateSSAOConstBuffer(Graphics& gfx, DirectX::XMMATRIX mView);


	ID3D11ShaderResourceView* pNormalMapSRV = nullptr;
	ID3D11ShaderResourceView* pRandomVectorsSRV = nullptr;
	ID3D11RenderTargetView* pNormalMapTRV = nullptr;
	//two for blur
	ID3D11RenderTargetView* pAmbientRTV0 = nullptr;
	ID3D11RenderTargetView* pAmbientRTV1 = nullptr;
	ID3D11ShaderResourceView* pAmbientSRV0 = nullptr;
	ID3D11ShaderResourceView* pAmbientSRV1 = nullptr;



	ID3D11Buffer* pQuadVertexBuffer = nullptr;
	ID3D11Buffer* pQuadIndexBuffer = nullptr;
	ID3D11Buffer* pSSAOConstBuffer = nullptr;
	ID3D11SamplerState* pRandomVectorSampler = nullptr;
	ID3D11SamplerState* pNormalMapSampler = nullptr;
	D3D11_VIEWPORT vp;

	DirectX::XMFLOAT4 frustumFarCorner[4];
	DirectX::XMFLOAT4 offsets[14];
	DirectX::XMMATRIX viewToTextureSpace;
};
