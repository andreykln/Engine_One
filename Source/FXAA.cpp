#include "FXAA.h"

FXAA::FXAA(Graphics& gfx)
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = resolution_width;
	texDesc.Height = resolution_height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* pTexture = nullptr;
	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateTexture2D(&texDesc, 0u, &pTexture));

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVdesc;
	SRVdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVdesc.Texture2D.MipLevels = texDesc.MipLevels;
	SRVdesc.Texture2D.MostDetailedMip = 0;
	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateShaderResourceView(pTexture, &SRVdesc, &pFXAA_SRV));

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;


}
