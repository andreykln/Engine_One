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


void Graphics::SetMatrices(const DirectX::XMMATRIX& ViewProjection, const DirectX::XMMATRIX& View, const DirectX::XMMATRIX& Projection)
{
	mView = View;
	mViewProjection = ViewProjection;
	mProjection = Projection;
}

void Graphics::CreateCBuffers()
{
	cbCreateNormalMap nMap;
	ID3D11Buffer* pNMap = CreateConstantBuffer(&nMap, sizeof(cbCreateNormalMap), L"normal map cBuffer");
	TESTCBUFFER = pNMap;
	constBuffersMap.insert(std::make_pair("NormalMap", pNMap));

	cbShadowMap smMap;
	ID3D11Buffer* pSMap = CreateConstantBuffer(&smMap, sizeof(cbShadowMap), L"Shadow map cBuffer");
	constBuffersMap.insert(std::make_pair("ShadowMap", pSMap));

	cbDefaultMatricesVS vsMatricesCB;
	ID3D11Buffer* pvsMatricesCB = CreateConstantBuffer(vsMatricesCB, sizeof(cbDefaultMatricesVS), L"Default VS with matrices CB");
	constBuffersMap.insert(std::make_pair("defaultVS", pvsMatricesCB));

	cbBlurSSAO ssaoBlurData;
	ID3D11Buffer* pssaoBlurData = CreateConstantBuffer(&ssaoBlurData, sizeof(cbBlurSSAO), L"SSAO blur settings");
	constBuffersMap.insert(std::make_pair("ssaoBlur", pssaoBlurData));

	cbComputeSSAO ssaoComputeMatrix;
	ID3D11Buffer* pssaoComputeMatrix = CreateConstantBuffer(&ssaoComputeMatrix, sizeof(cbComputeSSAO), L"ssao compute matrix");
	constBuffersMap.insert(std::make_pair("ssaoPerFrame", pssaoComputeMatrix));


}

void Graphics::CreateRuntimeCBuffers(cbComputeSSAOconstData& ssaoBuffer)
{
	ID3D11Buffer* pssaoBuff = CreateConstantBuffer(ssaoBuffer, sizeof(cbComputeSSAOconstData), L"Compute SSAO CB");
	constBuffersMap.insert(std::make_pair("ssaoConstData", pssaoBuff));

}

void Graphics::CreateSRVs()
{
	////Cubemap
	const size_t numOfCubeMaps = 4;
	std::vector<std::wstring> cubemapFiles;
	cubemapFiles.push_back(L"grasscube1024");
	cubemapFiles.push_back(L"desertcube1024");
	cubemapFiles.push_back(L"sunsetcube1024");
	cubemapFiles.push_back(L"snowcube1024");

	for (int i = 0; i < numOfCubeMaps; i++)
	{
		ID3D11ShaderResourceView* pTemp = nullptr;
		std::wstring path = L"Textures\\";
		path += cubemapFiles[i] + L".dds";
		pTemp = CreateSRV(path, true);
		cubeMaps.insert(std::make_pair(cubemapFiles[i], pTemp));
	}

	std::vector<std::wstring> diffuseMapNames;
	diffuseMapNames.push_back(L"bricks3");
	diffuseMapNames.push_back(L"flame");
	diffuseMapNames.push_back(L"flare");
	diffuseMapNames.push_back(L"flarealpha");
	diffuseMapNames.push_back(L"floor");
	diffuseMapNames.push_back(L"grass");
	diffuseMapNames.push_back(L"ice");
	diffuseMapNames.push_back(L"stones");
	diffuseMapNames.push_back(L"water1");
	diffuseMapNames.push_back(L"water2");
	diffuseMapNames.push_back(L"waves0");
	diffuseMapNames.push_back(L"WoodCrate01");

	for (int i = 0; i < diffuseMapNames.size(); i++)
	{
		ID3D11ShaderResourceView* pTemp = nullptr;
		std::wstring path = L"Textures\\";
		path += diffuseMapNames[i] + L".dds";
		pTemp = CreateSRV(path, false);
		diffuseMaps.insert(std::make_pair(diffuseMapNames[i], pTemp));
	}
	std::vector<std::wstring> normalMapNames;
	normalMapNames.push_back(L"bricks3_nmap");
	normalMapNames.push_back(L"floor_nmap");
	normalMapNames.push_back(L"stones_nmap");
	normalMapNames.push_back(L"waves0");
	normalMapNames.push_back(L"waves1");
	for (int i = 0; i < normalMapNames.size(); i++)
	{
		ID3D11ShaderResourceView* pTemp = nullptr;
		std::wstring path = L"Textures\\";
		path += normalMapNames[i] + L".dds";
		pTemp = CreateSRV(path, false);
		diffuseMaps.insert(std::make_pair(normalMapNames[i], pTemp));
	}

}


void Graphics::ConstBufferNormalMapBind()
{
	pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &constBuffersMap.at("NormalMap"));
}

void Graphics::NormalMap(const DirectX::XMMATRIX world)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(pgfx_pDeviceContext->Map(constBuffersMap.at("NormalMap"), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	cbCreateNormalMap* cBuffer = reinterpret_cast<cbCreateNormalMap*> (mappedData.pData);
	cBuffer->worldInvTransposeView = (MathHelper::InverseTranspose(world) * DirectX::XMMatrixTranspose(mView));
	cBuffer->worldView = DirectX::XMMatrixTranspose(world * mView);
	cBuffer->worldViewProjection = DirectX::XMMatrixTranspose(world * mViewProjection);
	pgfx_pDeviceContext->Unmap(constBuffersMap.at("NormalMap"), 0u);

}

void Graphics::ReleaseNormalMapResource()
{
	ID3D11ShaderResourceView* pNullSRV = nullptr;
	//random vector map
	pgfx_pDeviceContext->PSSetShaderResources(2u, 1u, &pNullSRV);
	//normal-depth map
	pgfx_pDeviceContext->PSSetShaderResources(3u, 1u, &pNullSRV);
}

void Graphics::ComputeSSAO(ID3D11RenderTargetView* pAmbientRTV0, D3D11_VIEWPORT& ssaoViewport,
	ID3D11ShaderResourceView* randomVecSRV, ID3D11ShaderResourceView* pNormalMapSRV)
{
	ID3D11ShaderResourceView* pNULLSRV = nullptr;
	//clear previous frame's binding
// 	pgfx_pDeviceContext->PSSetShaderResources(4u, 1u, &pNULLSRV);

	// Bind the ambient map as the render target.  Observe that this pass does not bind 
	// a depth/stencil buffer--it does not need it, and without one, no depth test is
	// performed, which is what we want.
	ID3D11RenderTargetView* renderTargets[1] = { pAmbientRTV0 };
	pgfx_pDeviceContext->OMSetRenderTargets(1u, &renderTargets[0], 0u);
	pgfx_pDeviceContext->ClearRenderTargetView(renderTargets[0], DirectX::Colors::Black);
	pgfx_pDeviceContext->RSSetViewports(1u, &ssaoViewport);


	DirectX::XMMATRIX projectionToTextureSpace = mProjection * toTexSpace;
	cbComputeSSAO temp;
	temp.viewToTexSpace = projectionToTextureSpace;
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(pgfx_pDeviceContext->Map(constBuffersMap.at("ssaoPerFrame"), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	cbComputeSSAO* pBuffer = reinterpret_cast<cbComputeSSAO*>(mappedData.pData);
	pBuffer->viewToTexSpace = DirectX::XMMatrixTranspose(projectionToTextureSpace);
	pgfx_pDeviceContext->Unmap(constBuffersMap.at("ssaoPerFrame"), 0u);

	pgfx_pDeviceContext->VSSetConstantBuffers(1u, 1u, &constBuffersMap.at("ssaoConstData"));
	pgfx_pDeviceContext->PSSetConstantBuffers(1u, 1u, &constBuffersMap.at("ssaoConstData"));
	pgfx_pDeviceContext->PSSetConstantBuffers(2u, 1u, &constBuffersMap.at("ssaoPerFrame"));

	pgfx_pDeviceContext->PSSetShaderResources(2u, 1u, &randomVecSRV);
	pgfx_pDeviceContext->PSSetShaderResources(3u, 1u, &pNormalMapSRV);


}

void Graphics::ConstBufferShadowMapBind()
{
	pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &constBuffersMap.at("ShadowMap"));
}

void Graphics::ShadowMap(const DirectX::XMMATRIX world, const DirectX::XMMATRIX& lightViewProj)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(pgfx_pDeviceContext->Map(constBuffersMap.at("ShadowMap"), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
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
	DX::ThrowIfFailed(pgfx_pDeviceContext->Map(constBuffersMap.at("defaultVS"), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
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
	pgfx_pDeviceContext->PSSetShaderResources(4u, 1u, &cubeMaps.at(skyBoxName));
}


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




void Graphics::BlurSSAOMap(int blurCount, ID3D11RenderTargetView* pAmbientMapRTV0, ID3D11RenderTargetView* pAmbientMapRTV1,
	ID3D11ShaderResourceView* pAmbientMapSRV0, ID3D11ShaderResourceView* pAmbientMapSRV1, D3D11_VIEWPORT ssaoViewPort)
{
	for (int i = 0; i < blurCount; i++)
	{
		// Ping-pong the two ambient map textures as we apply
		// horizontal and vertical blur passes.
		BlurSSAOMap(pAmbientMapSRV0, pAmbientMapRTV1, ssaoViewPort, true);
		BlurSSAOMap(pAmbientMapSRV1, pAmbientMapRTV0, ssaoViewPort, false);
	}
	//for reading in final draw calls
	pgfx_pDeviceContext->PSSetShaderResources(5u, 1u, &pAmbientMapSRV0);
// 	//release input for blurs
// 	ID3D11ShaderResourceView* pNULLSRV = nullptr;
// 	pgfx_pDeviceContext->PSSetShaderResources(0u, 1u, &pNULLSRV);

}

void Graphics::BlurSSAOMap(ID3D11ShaderResourceView* pInputSRV, ID3D11RenderTargetView* pOutputRTV,
	D3D11_VIEWPORT ssaoViewPort, bool horizontalBlur)
{
	ID3D11RenderTargetView* renderTargets[1] = { pOutputRTV };
	pgfx_pDeviceContext->OMSetRenderTargets(1, &renderTargets[0], 0);
	pgfx_pDeviceContext->ClearRenderTargetView(renderTargets[0], DirectX::Colors::Black);
	pgfx_pDeviceContext->RSSetViewports(1u, &ssaoViewPort);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(pgfx_pDeviceContext->Map(constBuffersMap.at("ssaoBlur"), 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	cbBlurSSAO* pBuffer = reinterpret_cast<cbBlurSSAO*>(mappedData.pData);
	if (horizontalBlur)
	{
		pBuffer->horizBool = true;
	}
	else
	{
		pBuffer->horizBool = false;
	}
	pBuffer->texelHeight = 1.0f / vp.Height;
	pBuffer->texelWidth = 1.0f / vp.Width;
	pgfx_pDeviceContext->Unmap(constBuffersMap.at("ssaoBlur"), 0u);

	pgfx_pDeviceContext->PSSetConstantBuffers(0u, 1u, &constBuffersMap.at("ssaoBlur"));
	pgfx_pDeviceContext->PSSetShaderResources(5u, 1u, &pInputSRV);

	//IA vertex buffer and index is already set from normal map
	pgfx_pDeviceContext->DrawIndexed(6, 0u, 0u);

	//unbind for the next blur pass
	ID3D11ShaderResourceView* pNULLSRV = nullptr;
	pgfx_pDeviceContext->PSSetShaderResources(5u, 1u, &pNULLSRV);
	ID3D11RenderTargetView* pNULLRTV = nullptr;
	renderTargets[0] = pNULLRTV;
	pgfx_pDeviceContext->OMSetRenderTargets(1, &renderTargets[0], 0);
}

ID3D11ShaderResourceView* Graphics::CreateSRV(std::wstring& in_path, bool cubeMap)
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
	if (cubeMap)
	{
		texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	}
	else
	{
		texDesc.MiscFlags = 0u;
	}
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
	if (cubeMap)
	{
		shaderResDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	}
	else
	{
		shaderResDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	}
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



//0-LinearWrap, 1-AnisotropicWrap, 2-ShadowMap, 3-NormalMap, 4-LinearClamp(ssao blur map)
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

	//Normal map 
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	// Set a very far depth value if sampling outside of the NormalDepth map
	// so we do not get false occlusions.
	samplerDesc.BorderColor[3] = 1e5f;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	ID3D11SamplerState* pST3 = nullptr;
	hr = pgfx_pDevice->CreateSamplerState(&samplerDesc, &pST3);
	samplers.push_back(pST3);

	//LinearClamp, SSAO blur map
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	ID3D11SamplerState* pST4 = nullptr;
	hr = pgfx_pDevice->CreateSamplerState(&samplerDesc, &pST4);
	samplers.push_back(pST4);

	//bind all samplers once and at the creation
	for (int i = 0; i < samplers.size(); i++)
	{
		pgfx_pDeviceContext->PSSetSamplers(i, 1u, &samplers[i]);
	}
}




