#include "ShadowMap.h"

ShadowMapGen::ShadowMapGen(Graphics& gfx, UINT width, UINT height)
	: mWidth(width), mHeight(height)
{
	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;
	mViewport.Width = mWidth;
	mViewport.Height = mHeight;
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
	sceneBounds.radius = sqrt(12.5f * 12.5f + 12.5f * 12.5f);

}

ID3D11ShaderResourceView* ShadowMapGen::DepthMapSRV()
{
	return pDepthMapSRV;
}

void ShadowMapGen::BindDSVandSetNullRenderTarget(Graphics& gfx)
{
	gfx.pgfx_pDeviceContext->RSSetViewports(1, &mViewport);

	ID3D11RenderTargetView* renderTargets[1] = { 0 };
	gfx.pgfx_pDeviceContext->OMSetRenderTargets(1, renderTargets, pDepthMapDSV);
	gfx.pgfx_pDeviceContext->ClearDepthStencilView(pDepthMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void ShadowMapGen::UpdateScene(float dt, DirectionalLight* oldLightDir)
{
	lightRotationAngle += 0.1f * dt;

	DirectX::XMMATRIX R = DirectX::XMMatrixRotationY(lightRotationAngle);

	for (int i = 0; i < 3; ++i)
	{
		DirectX::XMVECTOR lightDir = DirectX::XMLoadFloat3(&oldLightDir[i].direction);
		lightDir = DirectX::XMVector3TransformNormal(lightDir, R);
		DirectX::XMStoreFloat3(&oldLightDir[i].direction, lightDir);
	}
}

void ShadowMapGen::BuildShadowTransform(DirectionalLight* oldLightDir)
{
	using namespace DirectX;
	//only first light cast shadow
	XMVECTOR lightDir = DirectX::XMLoadFloat3(&oldLightDir[0].direction);
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
	XMStoreFloat4x4(&lightView, V);
	XMStoreFloat4x4(&lightProj, P);
	XMStoreFloat4x4(&shadowTransform, S);
}