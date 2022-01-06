#pragma once
#include "DrawableBase.h"
#include "directxpackedvector.h"
class Shader;
class SSAO : public Shape
{
public:
	SSAO(Graphics& gfx, UINT mWidth, UINT mHeight);
	ID3D11ShaderResourceView* GetNormalMapSRV();
	ID3D11ShaderResourceView* GetRandomVectorSRV();
	void ComputeSSAO(Graphics& gfx, DirectX::XMMATRIX mProj);
	void SetNormalDepthRenderTarget(Graphics& gfx, ID3D11DepthStencilView* dsv);
	void DrawDebugScreenQuad(Graphics& gfx, Shaders* shaders);
	void BlurAmbientMap(Graphics& gfx, int blurCount, Shaders* pShader);
	void SetSSAOMapToPS(Graphics& gfx);
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

	struct SSAOBlur
	{
		float texelWidth;
		float texelHeight;
		BOOL horizBool;
		int pad0;
	};

	void BuildRandomVectorTexture(Graphics& gfx);
	void BuildConstantBuffer(Graphics& gfx);
	void BuildTextureViewsAndViewport(Graphics& gfx, UINT mWidth, UINT mHeight);
	void BuildFrustumFarCorners(float fovy, float farZ);
	void BuildOffsetVectors();
	void BuildFullScreenQuadBuffers(Graphics& gfx);
	void BuildSamplers(Graphics& gfx);
	void BuildDebugScreenQuadData(Graphics& gfx);
	void BlurAmbientMap(Graphics& gfx, ID3D11ShaderResourceView* pInputSRV, ID3D11RenderTargetView* pOutputRTV, bool horizontalBlur);
	void UpdateSSAOConstBuffer(Graphics& gfx, DirectX::XMMATRIX mView);


	ID3D11ShaderResourceView* pNormalMapSRV = nullptr;
	ID3D11ShaderResourceView* pRandomVectorsSRV = nullptr;
	ID3D11RenderTargetView* pNormalMapRTV = nullptr;
	//two for blur
	ID3D11RenderTargetView* pAmbientRTV0 = nullptr;
	ID3D11RenderTargetView* pAmbientRTV1 = nullptr;
	ID3D11ShaderResourceView* pAmbientSRV0 = nullptr;
	ID3D11ShaderResourceView* pAmbientSRV1 = nullptr;



	ID3D11Buffer* pQuadVertexBuffer = nullptr;
	ID3D11Buffer* pQuadIndexBuffer = nullptr;
	ID3D11Buffer* pSSAOConstBuffer = nullptr;
	ID3D11Buffer* pSSAOBlurBuffer = nullptr;
	ID3D11SamplerState* pRandomVectorSampler = nullptr;
	ID3D11SamplerState* pNormalMapSampler = nullptr;
	ID3D11SamplerState* pBlurSampler = nullptr;
	ID3D11SamplerState* pSSAOMapSampler = nullptr;
	D3D11_VIEWPORT vp;
	SSAOBlur blurConstBuff;

	DirectX::XMFLOAT4 frustumFarCorner[4];
	DirectX::XMFLOAT4 offsets[14];
	DirectX::XMMATRIX viewToTextureSpace;
};
