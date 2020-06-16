#include "ShaderResourceView.h"
#include "DirectXTex/DirectXTex/DirectXTexP.h"
#include "DirectXTex/DDSTextureLoader/DDSTextureLoader11.h"
ShaderResourceView::ShaderResourceView(Graphics& gfx, const std::wstring& path)
{
	DirectX::ScratchImage* pImageData = new DirectX::ScratchImage();
	LoadFromDDSFile(path.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, *pImageData);
	
	const DirectX::TexMetadata& textureMetaData = pImageData->GetMetadata();
	DXGI_FORMAT textureFormat = textureMetaData.format;

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Format = textureFormat;
	texDesc.Width = (UINT)textureMetaData.width;
	texDesc.Height = (UINT)textureMetaData.height;
	texDesc.ArraySize = (UINT)textureMetaData.arraySize;
	texDesc.MipLevels = (UINT)textureMetaData.mipLevels;
	texDesc.SampleDesc.Count = 1u;
	texDesc.SampleDesc.Quality = 0u;

	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	texDesc.MiscFlags = 0u;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResDesc;
	shaderResDesc.Format = textureFormat;
	shaderResDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResDesc.Texture2D.MipLevels = -1; //to use all mip levels
	shaderResDesc.Texture2D.MostDetailedMip = (UINT)textureMetaData.mipLevels - 1;

	//sampler is here
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	DX::ThrowIfFailed(GetDevice(gfx)->CreateTexture2D(&texDesc, NULL, &pTexture));
	DX::ThrowIfFailed(GetDevice(gfx)->CreateShaderResourceView(pTexture, &shaderResDesc, &pShaderResourceView));
	DX::ThrowIfFailed(GetDevice(gfx)->CreateSamplerState(&samplerDesc, &pSamplerState));
	
}

void ShaderResourceView::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->PSSetShaderResources(0u, 1u, &pShaderResourceView);
	GetContext(gfx)->PSSetSamplers(0u, 1u, &pSamplerState);
}
