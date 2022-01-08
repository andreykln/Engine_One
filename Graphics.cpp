#include "Graphics.h"
#include "DirectXTex/DDSTextureLoader/DDSTextureLoader11.cpp"


Graphics::Graphics(HWND wnd)
{
	windowHandle = wnd;
	DXGI_SWAP_CHAIN_DESC swapChainDesc{ 0 };
	swapChainDesc.BufferDesc.Width = 0;
	swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1u;
	swapChainDesc.SampleDesc.Quality = 0u;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2u; 
	swapChainDesc.OutputWindow = wnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapChainDesc.Flags = 0;
	
	DX::ThrowIfFailed(D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
#ifdef MY_DEBUG
		D3D11_CREATE_DEVICE_DEBUG,
#endif
#ifndef MY_DEBUG
		0u,
#endif // !MY_DEBUG
		d3dFeatureLevels, 
		featureLevelNum, 
		D3D11_SDK_VERSION,
		&swapChainDesc,
		pgfx_SwapChain.ReleaseAndGetAddressOf(),
		pgfx_pDevice.ReleaseAndGetAddressOf(),
		&featureLevelIsSupported, 
		pgfx_pDeviceContext.ReleaseAndGetAddressOf()));
#ifdef MY_DEBUG
	pgfx_pDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugDevice));
	SetDeviceDebugName(pgfx_pDeviceContext.Get(), L"DeviceContextCreation.");
#endif


	D3D11_TEXTURE2D_DESC descDepthTexture;
	descDepthTexture.Width = resolution_width;
	descDepthTexture.Height = resolution_height;
	descDepthTexture.MipLevels = 1u;
	descDepthTexture.ArraySize = 1u;
	descDepthTexture.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //	DXGI_FORMAT_D32_FLOAT DXGI_FORMAT_D24_UNORM_S8_UINT
	descDepthTexture.SampleDesc.Count = 1u;
	descDepthTexture.SampleDesc.Quality = 0u;
	descDepthTexture.Usage = D3D11_USAGE_DEFAULT;
	descDepthTexture.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepthTexture.CPUAccessFlags = 0;
	descDepthTexture.MiscFlags = 0;
	DX::ThrowIfFailed(pgfx_pDevice->CreateTexture2D(&descDepthTexture, 0u, pgfx_TextureDepthStencil.ReleaseAndGetAddressOf()));
#ifdef MY_DEBUG
	SetDeviceDebugName(pgfx_TextureDepthStencil.Get(), L"Text2DDepthStentcil.");
#endif

	DX::ThrowIfFailed(pgfx_pDevice->CreateDepthStencilView(
															pgfx_TextureDepthStencil.Get(),
															0u,
															pgfx_DepthStencilView.ReleaseAndGetAddressOf()));
#ifdef MY_DEBUG
	SetDeviceDebugName(pgfx_DepthStencilView.Get(), L"DepthStencilView.");
#endif

	//viewport
	vp.Width = resolution_width;
	vp.Height = resolution_height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

#ifdef MY_DEBUG
	ID3D11InfoQueue* infoQueue = nullptr;
	pgfx_pDevice->QueryInterface(__uuidof(ID3D11InfoQueue), reinterpret_cast<void**>(&infoQueue));
	if (infoQueue)
	{
		infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_WARNING, FALSE);
		infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, TRUE);
		infoQueue->Release();
		infoQueue = nullptr;
	}

#endif
	DX::ThrowIfFailed(pgfx_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pgfx_BackBuffer));
	DX::ThrowIfFailed(pgfx_pDevice->CreateRenderTargetView(pgfx_BackBuffer.Get(), nullptr, pgfx_RenderTargetView.ReleaseAndGetAddressOf()));
#ifdef MY_DEBUG
	SetDeviceDebugName(pgfx_RenderTargetView.Get(), L"CreateRenderTargetView.");
#endif
}

Graphics::~Graphics()
{
#ifdef MY_DEBUG
	if (debugDevice != nullptr)
	{
		debugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}
	debugDevice->Release();
	debugDevice = nullptr;
#endif
}

void Graphics::EndFrame()
{
	DX::ThrowIfFailed(pgfx_SwapChain->Present(0u, 0u));
	pgfx_pDeviceContext->OMSetRenderTargets(1u, pgfx_RenderTargetView.GetAddressOf(), pgfx_DepthStencilView.Get());
	pgfx_pDeviceContext->RSSetViewports(1u, &vp);
}


void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] = { red, green, blue, 1.0f };
	pgfx_pDeviceContext->ClearRenderTargetView(pgfx_RenderTargetView.Get(), color);
	pgfx_pDeviceContext->ClearDepthStencilView(pgfx_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}


void Graphics::DrawIndexed(UINT count) const noexcept
{
	pgfx_pDeviceContext->DrawIndexed(count, 0u, 0u);
}

void Graphics::DrawIndexed(UINT count, UINT startIndexLocation, UINT startVertexLocation)
{
	pgfx_pDeviceContext->DrawIndexed(count, startIndexLocation, startVertexLocation);
}

void Graphics::Draw(UINT VertexCount, UINT StartVertexLocation) const noexcept
{
	pgfx_pDeviceContext->Draw(VertexCount, StartVertexLocation);
}

void Graphics::DrawIndexedTwo(UINT count, UINT StartIndexLocation, INT BaseVertexLocation) const noexcept
{
	pgfx_pDeviceContext->DrawIndexed(count, StartIndexLocation, BaseVertexLocation);
}


void Graphics::DrawInstancedIndexed(UINT count, UINT instanceCount, UINT startIndexLocation, int baseVertexLocation, UINT startInstanceLocation)
{
	pgfx_pDeviceContext->DrawIndexedInstanced(count, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
}

HWND Graphics::GetWindowHandle() const noexcept
{
	return windowHandle;
}

void Graphics::SetViewport()
{
	pgfx_pDeviceContext->RSSetViewports(1u, &vp);
}


void Graphics::SetMatrices(const DirectX::XMMATRIX& ViewProjection, const DirectX::XMMATRIX& View)
{
	mView = View;
	mViewProjection = ViewProjection;
}

void Graphics::CreateCBuffers()
{
	cbCreateNormalMap nMap;
	ID3D11Buffer* pNMap = CreateConstantBuffer(&nMap, sizeof(cbCreateNormalMap), L"normal map cBuffer");
	constBuffersMap.insert(std::make_pair("NormalMap", pNMap));

	cbShadowMap smMap;
	ID3D11Buffer* pSMap = CreateConstantBuffer(&smMap, sizeof(cbShadowMap), L"Shadow map cBuffer");
	constBuffersMap.insert(std::make_pair("ShadowMap", pSMap));

	cbDefaultMatricesVS vsMatricesCB;
	ID3D11Buffer* pvsMatricesCB = CreateConstantBuffer(vsMatricesCB, sizeof(cbDefaultMatricesVS), L"Default VS with matrices CB");
	constBuffersMap.insert(std::make_pair("defaultVS", pvsMatricesCB));
}

void Graphics::CreateSRVs()
{
	////Cubemap
	const size_t numOfCubeMaps = 4;
	std::vector<std::wstring> names;
	names.push_back(L"grasscube1024");
	names.push_back(L"desertcube1024");
	names.push_back(L"sunsetcube1024");
	names.push_back(L"snowcube1024");

	for (int i = 0; i < numOfCubeMaps; i++)
	{
		ID3D11ShaderResourceView* pTemp = nullptr;
		std::wstring path = L"Textures\\";
		path += names[i] + L".dds";
		pTemp = CreateSRVtoCubeMap(path);
		cubeMaps.insert(std::make_pair(names[i], pTemp));
	}
	//////////////////////////////////////////////////////////////////////////



}

void Graphics::CreateAndBindSamplers()
{
	D3D11_SAMPLER_DESC samplerDesc;
	//linear
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;

	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	ID3D11SamplerState* pST0 = nullptr;
	HRESULT hr = pgfx_pDevice->CreateSamplerState(&samplerDesc, &pST0);
	if (FAILED(hr))
	{
		std::wstring message = L"Failed Sampler creation";
		MessageBoxW(windowHandle, message.c_str(), NULL, MB_OK);
	}
	samplers.push_back(pST0);

	//anisotropic
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	ID3D11SamplerState* pST1 = nullptr;
	hr = pgfx_pDevice->CreateSamplerState(&samplerDesc, &pST1);
	if (FAILED(hr))
	{
		std::wstring message = L"Failed Sampler creation";
		MessageBoxW(windowHandle, message.c_str(), NULL, MB_OK);
	}
	samplers.push_back(pST1);

	//shadow sampler
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
	ID3D11SamplerState* pST2 = nullptr;
	hr = pgfx_pDevice->CreateSamplerState(&samplerDesc, &pST2);
	if (FAILED(hr))
	{
		std::wstring message = L"Failed Sampler creation";
		MessageBoxW(windowHandle, message.c_str(), NULL, MB_OK);
	}
	samplers.push_back(pST2);


	for (int i = 0; i < samplers.size(); i++)
	{
		pgfx_pDeviceContext->PSSetSamplers(i, 1u, &samplers[i]);
	}


}

void Graphics::ConstBufferNormalMapBind()
{
	pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &constBuffersMap.at("NormalMap"));
}

void Graphics::NormalMap(const DirectX::XMMATRIX world)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(pgfx_pDeviceContext->Map(constBuffersMap.at("NormalMap"), 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	cbCreateNormalMap* cBuffer = reinterpret_cast<cbCreateNormalMap*> (mappedData.pData);
	cBuffer->worldInvTransposeView = (MathHelper::InverseTranspose(world) * DirectX::XMMatrixTranspose(mView));
	cBuffer->worldView = DirectX::XMMatrixTranspose(world * mView);
	cBuffer->worldViewProjection = DirectX::XMMatrixTranspose(world * mViewProjection);
	pgfx_pDeviceContext->Unmap(constBuffersMap.at("NormalMap"), 0u);
}

void Graphics::ConstBufferShadowMapBind()
{
	pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &constBuffersMap.at("ShadowMap"));
}

void Graphics::ShadowMap(const DirectX::XMMATRIX world, const DirectX::XMMATRIX& lightViewProj)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(pgfx_pDeviceContext->Map(constBuffersMap.at("ShadowMap"), 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	cbShadowMap* pMatrices = reinterpret_cast<cbShadowMap*>(mappedData.pData);
	pMatrices->lightWVP = DirectX::XMMatrixTranspose(world * lightViewProj);
	pMatrices->texTransform = DirectX::XMMatrixIdentity();
	pgfx_pDeviceContext->Unmap(constBuffersMap.at("ShadowMap"), 0u);
}

void Graphics::ConstBufferVSMatricesBind()
{
	pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &constBuffersMap.at("defaultVS"));

}

void Graphics::VSDefaultMatricesUpdate(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX texTransform,
	const DirectX::XMMATRIX& shadowTransform, const DirectX::XMMATRIX& matTransform, const DirectX::XMFLOAT3& cameraPositon)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(pgfx_pDeviceContext->Map(constBuffersMap.at("defaultVS"), 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	cbDefaultMatricesVS* pMatrices = reinterpret_cast<cbDefaultMatricesVS*>(mappedData.pData);
	pMatrices->cameraPositon = cameraPositon;
	pMatrices->matTransform = DirectX::XMMatrixTranspose(matTransform);
	pMatrices->shadowTransform = DirectX::XMMatrixTranspose(shadowTransform);
	pMatrices->texTransform = DirectX::XMMatrixTranspose(texTransform);
	pMatrices->viewProjection = DirectX::XMMatrixTranspose(mViewProjection);
	pMatrices->world = DirectX::XMMatrixTranspose(world);
	pMatrices->worldInvTranspose = MathHelper::InverseTranspose(world);
	pMatrices->worldViewProjTex = DirectX::XMMatrixTranspose(world * mViewProjection * toTexSpace);
	pMatrices->enableDisplacementMapping = false;
	pgfx_pDeviceContext->Unmap(constBuffersMap.at("defaultVS"), 0u);
}

void Graphics::BindCubeMap(std::wstring& skyBoxName) const
{
	pgfx_pDeviceContext->PSSetShaderResources(3u, 1u, &cubeMaps.at(skyBoxName));
}

#ifdef MY_DEBUG

ID3D11Buffer* Graphics::CreateIndexBuffer(const std::vector<UINT> indices, const std::wstring& name)
{
	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.ByteWidth = (sizeof(UINT) * indices.size());
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0u;
	desc.MiscFlags = 0u;
	desc.StructureByteStride = sizeof(UINT);
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = indices.data();
	initData.SysMemPitch = 0u;
	initData.SysMemSlicePitch = 0u;

	ID3D11Buffer* pBuffer = nullptr;
	HRESULT hr =  pgfx_pDevice->CreateBuffer(&desc, &initData, &pBuffer);
	if (FAILED(hr))
	{
		std::wstring message = L"Failed Index Buffer creation of ";
		message += name;
		MessageBoxW(windowHandle, message.c_str(), NULL, MB_OK);
	}
	return pBuffer;
}

void Graphics::SetDebugName(ID3D11DeviceChild* child, const std::wstring& name)
{
	if (child != nullptr)
	{
		//convert wstring to const char*
		char* c_sName = new char[name.length()+2];
		c_sName[name.size()] = '\0';
		WideCharToMultiByte(CP_ACP, 0u, name.c_str(), -1, c_sName, (UINT)name.length(), NULL, NULL);

		child->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(name.size()) - 1, c_sName);
		delete[] c_sName;
	}
}

std::string Graphics::wstrTostr(const std::wstring& wstr)
{
	int size_needed = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.length() + 1), 0, 0, 0, 0);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.length() + 1), &strTo[0], size_needed, 0, 0);
	return strTo;
}

void Graphics::SetDeviceDebugName(ID3D11DeviceChild* child, const std::wstring& name)
{
	SetDebugName(child, name);
}



ID3D11ShaderResourceView* Graphics::CreateSRVtoCubeMap(std::wstring& in_path)
{
	CheckFileExistence(in_path);
	DirectX::TexMetadata textureMetaData;
	DirectX::ScratchImage* pImageData = new DirectX::ScratchImage();
	LoadFromDDSFile(in_path.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, *pImageData);
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
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
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
	ID3D11Texture2D* pTexture = nullptr;

	HRESULT hr = pgfx_pDevice->CreateTexture2D(&texDesc, textureInitData, &pTexture);
	if (FAILED(hr))
	{
		std::wstring message = L"Failed Texture2D creation of ";
		message += in_path;
		MessageBoxW(windowHandle, message.c_str(), NULL, MB_OK);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResDesc;
	shaderResDesc.Format = textureMetaData.format;
	shaderResDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	shaderResDesc.Texture2D.MipLevels = (UINT)textureMetaData.mipLevels;
	shaderResDesc.Texture2D.MostDetailedMip = 0u;

	ID3D11ShaderResourceView* pSRV = nullptr;

	hr = pgfx_pDevice->CreateShaderResourceView(pTexture, &shaderResDesc, &pSRV);
	if (FAILED(hr))
	{
		std::wstring message = L"Failed SRV Cubemap creation of ";
		message += in_path;
		MessageBoxW(windowHandle, message.c_str(), NULL, MB_OK);
	}

	delete[] textureInitData;
	pImageData->Release();
	
	return pSRV;
}

void Graphics::CheckFileExistence(Graphics& gfx, const std::wstring& path)
{
	if (!std::filesystem::exists(path.c_str()))
	{
		//can't append string literal message to this string for some reason. 
		//I'll leave it like that, it's better than nothing
		MessageBox(gfx.GetWindowHandle(), gfx.wstrTostr(path).c_str(), NULL, MB_OK);
	}
}

void Graphics::CheckFileExistence(Graphics* gfx, const std::wstring& path)
{
	if (!std::filesystem::exists(path.c_str()))
	{
		//can't append string literal message to this string for some reason. 
		//I'll leave it like that, it's better than nothing
		MessageBox(gfx->GetWindowHandle(), gfx->wstrTostr(path).c_str(), NULL, MB_OK);
	}
}

void Graphics::CheckFileExistence(const std::wstring& path)
{
	if (!std::filesystem::exists(path.c_str()))
	{
		std::wstring message = L"Missing DDS file for ";
		message += path;
		MessageBoxW(windowHandle, message.c_str(), NULL, MB_OK);
	}
}

#endif



