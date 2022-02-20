#include "ShadowMap.h"

ShadowMapGen::ShadowMapGen(Graphics& gfx, UINT width, UINT height)
	: mWidth(width), mHeight(height)
{
	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;
	mViewport.Width = (FLOAT)mWidth;
	mViewport.Height = (FLOAT)mHeight;
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;

	//typeless because DSV will interpret the bits as D24_UNORM_S8_UINT
	//and SRV as R24_UNORM_X8_TYPELESS
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = mWidth;
	texDesc.Height = mHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* depthMap = nullptr;
	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateTexture2D(&texDesc, 0u, &depthMap));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateDepthStencilView(depthMap, &dsvDesc, &pDepthMapDSV));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateShaderResourceView(depthMap, &srvDesc, &pDepthMapSRV));
	depthMap->Release();

	sceneBounds.center = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	//original size for temple demo
//	sceneBounds.radius = sqrt(12.5f * 12.5f + 12.5f * 12.5f);

	sceneBounds.radius = sqrt(25.0f * 25.0f + 25.0f * 25.0f);




	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;

	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateSamplerState(&samplerDesc, &pShadowSampler));

}

ID3D11ShaderResourceView** ShadowMapGen::DepthMapSRV()
{
	return &pDepthMapSRV;
}

void ShadowMapGen::SetShadowSampler(Graphics& gfx)
{
	gfx.pgfx_pDeviceContext->PSSetSamplers(1u, 1u, &pShadowSampler);
}

void ShadowMapGen::BindDSVandSetNullRenderTarget(Graphics& gfx)
{
	gfx.pgfx_pDeviceContext->RSSetViewports(1, &mViewport);

	ID3D11RenderTargetView* renderTargets[1] = { 0 };
	gfx.pgfx_pDeviceContext->OMSetRenderTargets(1, renderTargets, pDepthMapDSV);
	gfx.pgfx_pDeviceContext->ClearDepthStencilView(pDepthMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void ShadowMapGen::UpdateScene(float dt)
{
	lightRotationAngle += 0.2f * dt;
	newLightDirection = defaultLightDirection;
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationY(lightRotationAngle);
	DirectX::XMVECTOR lightDir = DirectX::XMLoadFloat3(&newLightDirection);
	lightDir = DirectX::XMVector3TransformNormal(lightDir, R);
	DirectX::XMStoreFloat3(&newLightDirection, lightDir);
}

void ShadowMapGen::UpdateSceneSponza(float dt)
{
// 	newLightDirection = defaultLightDirection;
	DirectX::XMVECTOR V0 = DirectX::XMLoadFloat3(&newLightDirection);
	DirectX::XMVECTOR V1 = DirectX::XMLoadFloat3(&defaultLightDirectionReflected);

	DirectX::XMVECTOR NV = DirectX::XMVectorLerp(V0, V1, sin(0.2 * dt));
	DirectX::XMStoreFloat3(&newLightDirection, NV);
}

DirectX::XMMATRIX ShadowMapGen::GetShadowTransform()
{
	return shadowTransform;
}

DirectX::XMMATRIX ShadowMapGen::GetLightProjection()
{
	return lightProj;
}

DirectX::XMMATRIX ShadowMapGen::GetLightView()
{
	return lightView;
}

DirectX::XMMATRIX ShadowMapGen::GetLighViewProjection() const
{
	return lightViewProjection;
}

DirectX::XMFLOAT3& ShadowMapGen::GetNewLightDirection()
{
	return newLightDirection;
}

void ShadowMapGen::BuildShadowTransform(DirectX::XMFLOAT3& oldLightDir)
{
	using namespace DirectX;
	XMVECTOR lightDir = DirectX::XMLoadFloat3(&oldLightDir);
	XMVECTOR lightPos = -2.0f * sceneBounds.radius * lightDir;
	XMVECTOR targetPos = XMLoadFloat3(&sceneBounds.center);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX V = XMMatrixLookAtLH(lightPos, targetPos, up);

	//transform bounding sphere to light space
	XMFLOAT3 sphereCenterLS;
	XMStoreFloat3(&sphereCenterLS, XMVector3TransformCoord(targetPos, V));

	//orhto frustum in light space enclosed scene
	float l = sphereCenterLS.x - sceneBounds.radius;
	float b = sphereCenterLS.y - sceneBounds.radius;
	float n = sphereCenterLS.z - sceneBounds.radius;
	float r = sphereCenterLS.x + sceneBounds.radius;
	float t = sphereCenterLS.y + sceneBounds.radius;
	float f = sphereCenterLS.z + sceneBounds.radius;
	XMMATRIX P = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

	//transform NDC space
	XMMATRIX T
	(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);
	XMMATRIX S = V * P * T;

	lightViewProjection = V * P;
	shadowTransform = S;
	lightProj = P;
	lightView = V;
}
