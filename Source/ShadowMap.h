#pragma once
#include "Shape.h"

class ShadowMapGen
{
public:
	struct BoundingSphereShadow
	{
		BoundingSphereShadow() : center(0.0f, 0.0f, 0.0f), radius(0.0f) {}
		DirectX::XMFLOAT3 center;
		float radius;
	};

	ShadowMapGen(Graphics& gfx, UINT width, UINT height);
	ID3D11ShaderResourceView** DepthMapSRV();
	void SetShadowSampler(Graphics& gfx);
	void BindDSVandSetNullRenderTarget(Graphics& gfx);
	void UpdateScene(float dt);
	void UpdateSceneSponza(float dt);
	void BuildShadowTransform(DirectX::XMFLOAT3& oldLightDir);
	DirectX::XMMATRIX GetShadowTransform();
	DirectX::XMMATRIX GetLightProjection();
	DirectX::XMMATRIX GetLightView();
	DirectX::XMMATRIX GetLighViewProjection() const;
	DirectX::XMFLOAT3& GetNewLightDirection();
private:
	float transl = 0.0f;
	UINT mWidth;
	UINT mHeight;
	BoundingSphereShadow sceneBounds;
	float lightRotationAngle;

	ID3D11ShaderResourceView* pDepthMapSRV = nullptr;
	ID3D11DepthStencilView* pDepthMapDSV = nullptr;
	ID3D11SamplerState* pShadowSampler = nullptr;

	D3D11_VIEWPORT mViewport;

	DirectX::XMFLOAT3 newLightDirection = defaultLightDirection;
	DirectX::XMMATRIX lightView;
	DirectX::XMMATRIX lightProj;
	DirectX::XMMATRIX shadowTransform;
	DirectX::XMMATRIX lightViewProjection;
};