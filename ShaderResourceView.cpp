#include "ShaderResourceView.h"
#include "DirectXTex/DirectXTex/DirectXTexP.h"
#include "DirectXTex/DDSTextureLoader/DDSTextureLoader11.h"
#include "DirectXTex/DDSTextureLoader/DDSTextureLoader11.cpp"

ShaderResourceView::ShaderResourceView(Graphics& gfx, const std::wstring& path)
{
	DirectX::ScratchImage* pImageData = new DirectX::ScratchImage();
	LoadFromDDSFile(path.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, *pImageData);
	const DirectX::TexMetadata& textureMetaData = pImageData->GetMetadata();
	DXGI_FORMAT textureFormat = textureMetaData.format;
// 	DirectX::CreateDDSTextureFromFile(
// 		GetDevice(gfx),
// 		GetContext(gfx),
// 		path.c_str(),
// 		&pResource,
// 		&pShaderResourceView,
// 		NULL,
// 		NULL);
	

	const DirectX::Image* image = pImageData->GetImage(0, 0, 0);
// 	DirectX::CreateTexture(GetDevice(gfx),
// 		image,
// 		10,
// 		textureMetaData,
// 		&pResource1);

	DirectX::CreateShaderResourceView(
	GetDevice(gfx),
		image, 10,
		textureMetaData,
		&pShaderResourceView);

// 	D3D11_TEXTURE2D_DESC texDesc;
// 	texDesc.Format = textureFormat;
// 	texDesc.Width = (UINT)textureMetaData.width;
// 	texDesc.Height = (UINT)textureMetaData.height;
// 	texDesc.ArraySize = (UINT)textureMetaData.arraySize;
// 	texDesc.MipLevels = (UINT)textureMetaData.mipLevels;
// 	texDesc.SampleDesc.Count = 1u;
// 	texDesc.SampleDesc.Quality = 0u;
// 
// 	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
// 	texDesc.CPUAccessFlags = 0u;
// 	texDesc.MiscFlags = 0u;
// 	texDesc.Usage = D3D11_USAGE_DEFAULT;
// 	D3D11_SUBRESOURCE_DATA textureInitData;
// 	textureInitData.pSysMem = image->pixels;
// 	textureInitData.SysMemPitch = image->rowPitch;
// 	textureInitData.SysMemSlicePitch = 0u;
// 
  //	DX::ThrowIfFailed(GetDevice(gfx)->CreateTexture2D(&texDesc, &textureInitData, &pTexture));


// 
// 	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResDesc;
// 	shaderResDesc.Format = textureFormat;
// 	shaderResDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
// 	shaderResDesc.Texture2D.MipLevels = -1; //to use all mip levels
// 	shaderResDesc.Texture2D.MostDetailedMip = (UINT)textureMetaData.mipLevels - 1;
// 	DX::ThrowIfFailed(GetDevice(gfx)->CreateShaderResourceView(pResource1, &shaderResDesc, &pShaderResourceView));

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	DX::ThrowIfFailed(GetDevice(gfx)->CreateSamplerState(&samplerDesc, &pSamplerState));
 	pImageData->Release();
}

void ShaderResourceView::Bind(Graphics& gfx) noexcept
{
 	GetContext(gfx)->PSSetShaderResources(0u, 1u, &pShaderResourceView);
	GetContext(gfx)->PSSetSamplers(0u, 1u, &pSamplerState);
}
