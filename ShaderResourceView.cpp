#include "ShaderResourceView.h"
#include <filesystem>
#include <cassert>
// #include "DirectXTex/DirectXTex/DirectXTexP.h"
// #include "DirectXTex/DDSTextureLoader/DDSTextureLoader11.h"
#include "DirectXTex/DDSTextureLoader/DDSTextureLoader11.cpp"

//create SRV for each texture
ShaderResourceView::ShaderResourceView(Graphics& gfx, std::wstring* in_path, UINT in_NumSRVs)
	: numSRVs{in_NumSRVs}, path{in_path}
{
#ifdef MY_DEBUG
	for (size_t i = 0; i < numSRVs; i++)
	{
		gfx.CheckFileExistence(gfx, path[i]);
	}
	
#endif // MY_DEBUG
	for (size_t i = 0; i < numSRVs; ++i)
	{
		DirectX::ScratchImage* pImageData = new DirectX::ScratchImage();
		LoadFromDDSFile(path[i].c_str(), DirectX::DDS_FLAGS_NONE, nullptr, *pImageData);
		textureMetaData = pImageData->GetMetadata();
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
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResDesc;
		shaderResDesc.Format = textureMetaData.format;
		shaderResDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D; //D3D11_SRV_DIMENSION_TEXTURE2D
		shaderResDesc.Texture2D.MipLevels = (UINT)textureMetaData.mipLevels;
		shaderResDesc.Texture2D.MostDetailedMip = 0u;
		DX::ThrowIfFailed(GetDevice(gfx)->CreateShaderResourceView(
			reinterpret_cast<ID3D11Resource*>(pTexture),
			&shaderResDesc,
			&pSRVArray[i]));
		delete[] textureInitData;
		pImageData->Release();
	}
}
//create texture array
ShaderResourceView::ShaderResourceView(Graphics& gfx, std::wstring* in_path, UINT in_NumofTextures, bool texarr)
: arrPath{in_path}, numTextures (in_NumofTextures)
{
#ifdef MY_DEBUG
	for (size_t i = 0; i < numTextures; i++)
	{
		gfx.CheckFileExistence(gfx, arrPath[i]);
	}

#endif // MY_DEBUG
	DirectX::Image* ImagesArray = new DirectX::Image[60];

	for (size_t i = 0; i < numTextures; ++i)
	{
		//no delete here, because ScratchImage have Release() inside
		DirectX::ScratchImage* pImageData = new DirectX::ScratchImage();
		LoadFromDDSFile(arrPath[i].c_str(), DirectX::DDS_FLAGS_NONE, &textureMetaData, *pImageData);
		const DirectX::Image* image = pImageData->GetImage(0, 0, 0);

// 		textureMetaData = pImageData->GetMetadata();
// 		DXGI_FORMAT textureFormat = textureMetaData.format;
// 		ImagesArray[i] = pImageData->GetImage(0, 0, 0);
		// 		const DirectX::Image* image = pImageData->GetImage(0, 0, 0);
		ImagesArray[i].format = textureMetaData.format;
		ImagesArray[i].height = textureMetaData.height;
		ImagesArray[i].rowPitch = image->rowPitch;
		ImagesArray[i].slicePitch = image->slicePitch;
		ImagesArray[i].width = textureMetaData.width;
		ImagesArray[i].pixels = image->pixels;

	}
	ScratchImage image;
	image.InitializeArrayFromImages(ImagesArray, numTextures);
// 	for (size_t i = 0; i < numTextures; i++)
// 	{
// 		if (ImagesArray[i].pixels != nullptr)
// 		{
// 			ImagesArray[i].pixels = nullptr;
// 			delete[] ImagesArray[i].pixels;
// 		}
// 	}
	textureMetaData.arraySize = numTextures;
	DirectX::CreateShaderResourceView(GetDevice(gfx), ImagesArray, numTextures, textureMetaData, &pSRVTexArray);
// 				D3D11_SHADER_RESOURCE_VIEW_DESC shaderResDesc;
// 			shaderResDesc.Format = textureMetaData.format;
// 			shaderResDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
// 			shaderResDesc.Texture2D.MipLevels = (UINT)textureMetaData.mipLevels;
// 			shaderResDesc.Texture2D.MostDetailedMip = 0u;
// 			shaderResDesc.Texture2DArray.ArraySize = -1; //with -1 its working, but not being an array
// 			shaderResDesc.Texture2DArray.FirstArraySlice = 0;
// 			shaderResDesc.Texture2DArray.MipLevels = 1;
// 			shaderResDesc.Texture2DArray.MostDetailedMip = 0;
// 			DX::ThrowIfFailed(GetDevice(gfx)->CreateShaderResourceView(
// 				reinterpret_cast<ID3D11Resource*>(*textureCarray),
// 				&shaderResDesc,
// 				&pSRVTexArray));

	/*
	
		for (size_t i = 0; i < numTextures; ++i)
	{
		DirectX::ScratchImage* pImageData = new DirectX::ScratchImage();
		LoadFromDDSFile(arrPath[i].c_str(), DirectX::DDS_FLAGS_NONE, nullptr, *pImageData);
		textureMetaData = pImageData->GetMetadata();
		DXGI_FORMAT textureFormat = textureMetaData.format;
		imagesArrayC[i] = pImageData->GetImage(0, 0, 0);
// 		const DirectX::Image* image = pImageData->GetImage(0, 0, 0);
	}
	
	*/
//  	pImageArray->Release();
	/////automatic creation of shader resource view including texture.
	/*DirectX::CreateShaderResourceView(
		GetDevice(gfx),
		, textureMetaData.mipLevels,
		textureMetaData,
		&pShaderResourceView);
	////end of automatic
	for (size_t i = 0; i < numTextures; ++i)
	{
		DirectX::ScratchImage* pImageData = new DirectX::ScratchImage();
		LoadFromDDSFile(arrPath[i].c_str(), DirectX::DDS_FLAGS_NONE, nullptr, *pImageData);
		textureMetaData = pImageData->GetMetadata();
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
		texDesc.ArraySize = numTextures; //(UINT)textureMetaData.arraySize
		texDesc.MipLevels = (UINT)textureMetaData.mipLevels;
		texDesc.SampleDesc.Count = 1u;
		texDesc.SampleDesc.Quality = 0u;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0u;
		texDesc.MiscFlags = 0u;
		texDesc.Usage = D3D11_USAGE_DEFAULT;

		const size_t amountOfTextures = textureMetaData.mipLevels * 60; //textureMetaData.mipLevels * textureMetaData.arraySize
		/*D3D11_SUBRESOURCE_DATA* textureInitData = new D3D11_SUBRESOURCE_DATA[amountOfTextures];

		size_t idx = 0;
		for (size_t item = 0; item < 60; ++item) //size_t item = 0; item < textureMetaData.arraySize; ++item
		{
			for (size_t level = 0; level < textureMetaData.mipLevels; ++level)
			{
				size_t index = textureMetaData.ComputeIndex(level, item, 0);
				const Image& img = image[index];
				assert(idx < (textureMetaData.mipLevels* 60)); //textureMetaData.mipLevels* textureMetaData.arraySize

				textureInitData[idx].pSysMem = img.pixels;
				textureInitData[idx].SysMemPitch = static_cast<DWORD>(img.rowPitch);
				textureInitData[idx].SysMemSlicePitch = static_cast<DWORD>(img.slicePitch);
				++idx;
			}
		}

		DX::ThrowIfFailed(GetDevice(gfx)->CreateTexture2D(&texDesc, textureInitData, &pTexture));
		textureCarray[i] = pTexture; //don't need it

		textureArray.push_back(pTexture); // maybe delete
		delete[] textureInitData;
		pImageData->Release();

		if (i == (numTextures - 1))
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResDesc;
			shaderResDesc.Format = textureMetaData.format;
			shaderResDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			shaderResDesc.Texture2D.MipLevels = (UINT)textureMetaData.mipLevels;
			shaderResDesc.Texture2D.MostDetailedMip = 0u;
			shaderResDesc.Texture2DArray.ArraySize = -1; //with -1 its working, but not being an array
			shaderResDesc.Texture2DArray.FirstArraySlice = 0;
			shaderResDesc.Texture2DArray.MipLevels = 1;
			shaderResDesc.Texture2DArray.MostDetailedMip = 0;
			DX::ThrowIfFailed(GetDevice(gfx)->CreateShaderResourceView(
				reinterpret_cast<ID3D11Resource*>(*textureCarray),
				&shaderResDesc,
				&pSRVTexArray));
		}

	}*/
}

void ShaderResourceView::Bind(Graphics& gfx) noexcept
{
// 	GetContext(gfx)->PSSetShaderResources(0u, numSRVs, pSRVArray);
	GetContext(gfx)->PSSetShaderResources(0u, 1u, &pSRVTexArray);

}
