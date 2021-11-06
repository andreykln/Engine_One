#pragma once
#include "Shape.h"

class ShadowMapGen : public Shape 
{
public:
	struct BoundingSphereShadow
	{
		BoundingSphereShadow() : center(0.0f, 0.0f, 0.0f), radius(0.0f) {}
		DirectX::XMFLOAT3 center;
		float radius;
	};

	ShadowMapGen(Graphics& gfx, UINT width, UINT height);
	ID3D11ShaderResourceView* DepthMapSRV();
	void BindDSVandSetNullRenderTarget(Graphics& gfx);
	void UpdateScene(float dt, DirectionalLight* oldLightDir);

private:
	void BuildShadowTransform(DirectionalLight* oldLightDir);

	UINT mWidth;
	UINT mHeight;
	BoundingSphereShadow sceneBounds;
	float lightRotationAngle;

	ID3D11ShaderResourceView* pDepthMapSRV = nullptr;
	ID3D11DepthStencilView* pDepthMapDSV = nullptr;

	D3D11_VIEWPORT mViewport;

	DirectX::XMFLOAT4X4 lightView;
	DirectX::XMFLOAT4X4 lightProj;
	DirectX::XMFLOAT4X4 shadowTransform;

};