#include "SSAO.h"

SSAO::SSAO(Graphics& gfx, UINT mWidth, UINT mHeight)
{
	//create texture for rendering view space normal vectors
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = mWidth;
	texDesc.Height = mHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* normalDepthTexture = nullptr;
	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateTexture2D(&texDesc, 0u, &normalDepthTexture));
	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateShaderResourceView(normalDepthTexture, 0u, &pNormalMapSRV));
	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateRenderTargetView(normalDepthTexture, 0u, &pRTV));
	normalDepthTexture->Release();




}

ID3D11RenderTargetView* SSAO::GetSSAO_RTV()
{
	return pRTV;
}

void SSAO::SetNormalDepthRenderTarget(Graphics& gfx, ID3D11DepthStencilView* dsv)
{
	ID3D11RenderTargetView* renderTargets[1] = { GetSSAO_RTV() };
	gfx.pgfx_pDeviceContext->OMSetRenderTargets(1, renderTargets, dsv);
	// Clear view space normal to (0,0,-1) and clear depth to be very far away.  
	float clearColor[] = { 0.0f, 0.0f, -1.0f, 1e5f };
	gfx.pgfx_pDeviceContext->ClearRenderTargetView(GetSSAO_RTV(), clearColor);
}
