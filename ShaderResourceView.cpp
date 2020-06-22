#include "ShaderResourceView.h"
#include <filesystem>
#include <cassert>
#include "DirectXTex/DirectXTex/DirectXTexP.h"
#include "DirectXTex/DDSTextureLoader/DDSTextureLoader11.h"
#include "DirectXTex/DDSTextureLoader/DDSTextureLoader11.cpp"

ShaderResourceView::ShaderResourceView(Graphics& gfx, const std::wstring& path)
{
	std::filesystem::exists(path.c_str());
	DirectX::ScratchImage* pImageData = new DirectX::ScratchImage();
	LoadFromDDSFile(path.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, *pImageData);
	const DirectX::TexMetadata& textureMetaData = pImageData->GetMetadata();
	DXGI_FORMAT textureFormat = textureMetaData.format;
	const DirectX::Image* image = pImageData->GetImage(0, 0, 0);

	/////automatic creation of shader resource view including texture.
// 	DirectX::CreateShaderResourceView(
// 		GetDevice(gfx),
// 		image, textureMetaData.mipLevels,
// 		textureMetaData,
// 		&pShaderResourceView);
	////end of automatic


 	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Format = textureFormat;
	texDesc.Width = (UINT)textureMetaData.width;
	texDesc.Height = (UINT)textureMetaData.height;
	texDesc.ArraySize = (UINT)textureMetaData.arraySize;
	texDesc.MipLevels = (UINT)textureMetaData.mipLevels;
	texDesc.SampleDesc.Count = 1u;
	texDesc.SampleDesc.Quality = 0u;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0u;
	texDesc.MiscFlags = 0u;
	texDesc.Usage = D3D11_USAGE_DEFAULT;

	const size_t amountOfTextures = textureMetaData.mipLevels * textureMetaData.arraySize;
	D3D11_SUBRESOURCE_DATA* textureInitData = new D3D11_SUBRESOURCE_DATA[amountOfTextures];

	size_t idx = 0;
	for (size_t item = 0; item < textureMetaData.arraySize; ++item)
	{
		for (size_t level = 0; level < textureMetaData.mipLevels; ++level)
		{
			size_t index = textureMetaData.ComputeIndex(level, item, 0);
			const Image& img = image[index];
			assert(idx < (textureMetaData.mipLevels* textureMetaData.arraySize));

			textureInitData[idx].pSysMem = img.pixels;
			textureInitData[idx].SysMemPitch = static_cast<DWORD>(img.rowPitch);
			textureInitData[idx].SysMemSlicePitch = static_cast<DWORD>(img.slicePitch);
			++idx;
		}
	}
  	DX::ThrowIfFailed(GetDevice(gfx)->CreateTexture2D(&texDesc, textureInitData, &pTexture));
	delete[] textureInitData;


	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResDesc;
	shaderResDesc.Format = textureFormat;
	shaderResDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResDesc.Texture2D.MipLevels = (UINT)textureMetaData.mipLevels;
	shaderResDesc.Texture2D.MostDetailedMip = 0u;
	DX::ThrowIfFailed(GetDevice(gfx)->CreateShaderResourceView(
		reinterpret_cast<ID3D11Resource*>(pTexture),
		&shaderResDesc,
		&pShaderResourceView));

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
 	samplerDesc.MipLODBias = 0.0f;
 	samplerDesc.MaxAnisotropy = 16;
 	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
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
