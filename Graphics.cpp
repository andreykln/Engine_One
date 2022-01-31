#include "Graphics.h"
#include "DirectXTex/DDSTextureLoader/DDSTextureLoader11.cpp"

#define ReleaseID3D(x) { if(x){ x->Release(); x = 0; } }

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
	InitShaders();
	InitializeRenderStates();
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



HWND Graphics::GetWindowHandle() const noexcept
{
	return windowHandle;
}

void Graphics::SetViewport()
{
	pgfx_pDeviceContext->RSSetViewports(1u, &vp);
}


void Graphics::SetCommonShaderConstants(const DirectX::XMMATRIX& ViewProjection, const DirectX::XMMATRIX& View,
	const DirectX::XMMATRIX& Projection, const DirectX::XMMATRIX& lightViewProjection , const DirectX::XMFLOAT3 camPos, float dt, float totalTime)
{
	mView = View;
	mViewProjection = ViewProjection;
	mProjection = Projection;
	mCameraPosition = camPos;
	mDeltaTime = dt;
	mTotalTime = totalTime;
	mLightViewProjection = lightViewProjection;
}

void Graphics::SetShadowTransform(const DirectX::XMMATRIX& shadowTransform)
{
	mShadowTransform = shadowTransform;
}

void Graphics::UpdateLightDirection(const DirectX::XMFLOAT3& newLightDirection)
{
	mNewLightDirection = newLightDirection;
}

void Graphics::CreateCBuffers()
{
	cbCreateNormalMap nMap;
	ID3D11Buffer* pNMap = CreateConstantBuffer(nMap, true, "normal map cBuffer");
	constBuffersMap.insert(std::make_pair(cbNames.normalMap, pNMap));

	cbShadowMap smMap;
	ID3D11Buffer* pSMap = CreateConstantBuffer(smMap, true, "Shadow map cBuffer");
	constBuffersMap.insert(std::make_pair(cbNames.shadowMap, pSMap));

	cbDefaultMatricesVS vsMatricesCB;
	ID3D11Buffer* pvsMatricesCB = CreateConstantBuffer(vsMatricesCB, true, "Default VS with matrices CB");
	constBuffersMap.insert(std::make_pair(cbNames.defaultVS, pvsMatricesCB));

	cbBlurSSAO ssaoBlurData;
	ID3D11Buffer* pssaoBlurData = CreateConstantBuffer(ssaoBlurData, true, "SSAO blur settings");
	constBuffersMap.insert(std::make_pair(cbNames.ssaoBlur, pssaoBlurData));

	cbComputeSSAO ssaoComputeMatrix;
	ID3D11Buffer* pssaoComputeMatrix = CreateConstantBuffer(ssaoComputeMatrix, true, "ssao compute matrix");
	constBuffersMap.insert(std::make_pair(cbNames.ssaoPerFrame, pssaoComputeMatrix));


	cbDefaultLightPSPerFrame defLightPerFrame;
	ID3D11Buffer* pDefLightPF = CreateConstantBuffer(defLightPerFrame, true, "default light per frame");
	constBuffersMap.insert(std::make_pair(cbNames.defaultLightPerFrame, pDefLightPF));

	cbDefaultLightPSdata defLight;
	ID3D11Buffer* pDefLight = CreateConstantBuffer(defLight, false, "default light constant data");
	constBuffersMap.insert(std::make_pair(cbNames.defaultLightData, pDefLight));

	cbVSTesselationWaves tessWavesVS;
	ID3D11Buffer* pTessWavesVS = CreateConstantBuffer(tessWavesVS, true, "tessellation waves vertex shader CB");
	constBuffersMap.insert(std::make_pair(cbNames.tessWavesMatrices, pTessWavesVS));

	cbHSTerrainPerFrame terrainHS;
	ID3D11Buffer* pterrHS = CreateConstantBuffer(terrainHS, true, "terrain world view planes");
	constBuffersMap.insert(std::make_pair(cbNames.terrainHSPlainsData, pterrHS));

	cbParticleStreamOutGS particleSOGS;
	ID3D11Buffer* pPartSOGS = CreateConstantBuffer(particleSOGS, true, "Particle stream out GS");
	constBuffersMap.insert(std::make_pair(cbNames.particleStreamOutGS, pPartSOGS));

	cbWavesUpdateCS computeWavesPerFrame;
	ID3D11Buffer* pCWavesPF = CreateConstantBuffer(computeWavesPerFrame, true, "Compute waves per frame constants");
	constBuffersMap.insert(std::make_pair(cbNames.computeWavesCSPerFrame, pCWavesPF));

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
	diffuseMapNames.push_back(L"snow");
	diffuseMapNames.push_back(L"WoodCrate01");
	diffuseMapNames.push_back(L"blend");
	diffuseMapNames.push_back(L"raindrop");

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
		normalMaps.insert(std::make_pair(normalMapNames[i], pTemp));
	}

	//terrain layer maps
	const int numOfTex = 4;
	std::wstring layers[numOfTex];
	layers[0] = L"Textures\\Terrain\\grass.dds";;
	layers[1] = L"Textures\\Terrain\\darkdirt.dds";
	layers[2] = L"Textures\\Terrain\\stone.dds";
	layers[3] = L"Textures\\Terrain\\lightdirt.dds";
// 	ID3D11ShaderResourceView* pLayerMaps = nullptr;
	ID3D11ShaderResourceView** pSRVArray = new ID3D11ShaderResourceView* [numOfTex];

	CreateSRVArray(&pSRVArray[0], 4, layers);
	diffuseMapArray.insert(std::make_pair(L"TerrainLayerMaps", &pSRVArray[0]));



}


void Graphics::AddSRVToMap(const std::wstring& name, ID3D11ShaderResourceView* pSRV, bool diffuse, bool normal)
{
	if (diffuse)
	{
		diffuseMaps.insert(std::make_pair(name, pSRV));
	}
	if (normal)
	{
		normalMaps.insert(std::make_pair(name, pSRV));
	}
}

void Graphics::ConstBufferNormalMapBind()
{
	pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &constBuffersMap.at("NormalMap"));
}

void Graphics::NormalMap(const DirectX::XMMATRIX world)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(pgfx_pDeviceContext->Map(constBuffersMap.at(cbNames.normalMap), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	cbCreateNormalMap* cBuffer = reinterpret_cast<cbCreateNormalMap*> (mappedData.pData);
	cBuffer->worldInvTransposeView = (MathHelper::InverseTranspose(world) * DirectX::XMMatrixTranspose(mView));
	cBuffer->worldView = DirectX::XMMatrixTranspose(world * mView);
	cBuffer->worldViewProjection = DirectX::XMMatrixTranspose(world * mViewProjection);
	pgfx_pDeviceContext->Unmap(constBuffersMap.at(cbNames.normalMap), 0u);

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
	DX::ThrowIfFailed(pgfx_pDeviceContext->Map(constBuffersMap.at(cbNames.ssaoPerFrame), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	cbComputeSSAO* pBuffer = reinterpret_cast<cbComputeSSAO*>(mappedData.pData);
	pBuffer->viewToTexSpace = DirectX::XMMatrixTranspose(projectionToTextureSpace);
	pgfx_pDeviceContext->Unmap(constBuffersMap.at(cbNames.ssaoPerFrame), 0u);

	pgfx_pDeviceContext->VSSetConstantBuffers(1u, 1u, &constBuffersMap.at(cbNames.ssaoConstData));
	pgfx_pDeviceContext->PSSetConstantBuffers(1u, 1u, &constBuffersMap.at(cbNames.ssaoConstData));
	pgfx_pDeviceContext->PSSetConstantBuffers(2u, 1u, &constBuffersMap.at(cbNames.ssaoPerFrame));

	pgfx_pDeviceContext->PSSetShaderResources(2u, 1u, &randomVecSRV);
	pgfx_pDeviceContext->PSSetShaderResources(3u, 1u, &pNormalMapSRV);


}

void Graphics::ConstBufferShadowMapBind()
{
	pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &constBuffersMap.at(cbNames.shadowMap));
}

void Graphics::ShadowMap(const DirectX::XMMATRIX world, const DirectX::XMMATRIX& lightViewProj)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(pgfx_pDeviceContext->Map(constBuffersMap.at(cbNames.shadowMap), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	cbShadowMap* pMatrices = reinterpret_cast<cbShadowMap*>(mappedData.pData);
	pMatrices->lightWVP = DirectX::XMMatrixTranspose(world * lightViewProj);
	pMatrices->texTransform = DirectX::XMMatrixIdentity();
	pgfx_pDeviceContext->Unmap(constBuffersMap.at(cbNames.shadowMap), 0u);
}

void Graphics::ConstBufferVSMatricesBind()
{
	pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &constBuffersMap.at(cbNames.defaultVS));

}

void Graphics::VSDefaultMatricesUpdate(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX texTransform, const DirectX::XMMATRIX& matTransform)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(pgfx_pDeviceContext->Map(constBuffersMap.at(cbNames.defaultVS), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	cbDefaultMatricesVS* pMatrices = reinterpret_cast<cbDefaultMatricesVS*>(mappedData.pData);
	pMatrices->cameraPosition = mCameraPosition;
	pMatrices->matTransform = DirectX::XMMatrixTranspose(matTransform);
	pMatrices->shadowTransform = DirectX::XMMatrixTranspose(mShadowTransform);
	pMatrices->texTransform = DirectX::XMMatrixTranspose(texTransform);
	pMatrices->viewProjection = DirectX::XMMatrixTranspose(mViewProjection);
	pMatrices->world = DirectX::XMMatrixTranspose(world);
	pMatrices->worldInvTranspose = MathHelper::InverseTranspose(world);
	pMatrices->worldViewProjTex = DirectX::XMMatrixTranspose(world * mViewProjection * toTexSpace);
	pMatrices->enableDisplacementMapping = false;
	pgfx_pDeviceContext->Unmap(constBuffersMap.at(cbNames.defaultVS), 0u);
}



void Graphics::SetDispWavesShaderRes(const std::wstring& normalMap0, const std::wstring& normalMap1, const std::wstring& diffuseMap)
{
	pgfx_pDeviceContext->DSSetShaderResources(0u, 1u, &normalMaps.at(normalMap0));
	pgfx_pDeviceContext->DSSetShaderResources(1u, 1u, &normalMaps.at(normalMap1));
	pgfx_pDeviceContext->DSSetConstantBuffers(0u, 1u, &constBuffersMap.at(cbNames.defaultVS));
	pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &constBuffersMap.at(cbNames.tessWavesMatrices));

	pgfx_pDeviceContext->PSSetShaderResources(0u, 1u, &diffuseMaps.at(diffuseMap));
	pgfx_pDeviceContext->PSSetShaderResources(1u, 1u, &normalMaps.at(normalMap0));
	pgfx_pDeviceContext->PSSetShaderResources(2u, 1u, &normalMaps.at(normalMap1));


}

void Graphics::UpdateDispWavesCBuffers(const DirectX::XMMATRIX& world, MaterialEx& mat)
{
	DirectX::XMFLOAT2 waveDispOffset0 = DirectX::XMFLOAT2(0.0f, 0.0f);
	DirectX::XMFLOAT2 waveDispOffset1 = DirectX::XMFLOAT2(0.0f, 0.0f);
	DirectX::XMFLOAT2 waveNormalOffset0 = DirectX::XMFLOAT2(0.0f, 0.0f);
	DirectX::XMFLOAT2 waveNormalOffset1 = DirectX::XMFLOAT2(0.0f, 0.0f);

	DirectX::XMMATRIX waveDispTexTransform0;
	DirectX::XMMATRIX waveDispTexTransform1;
	DirectX::XMMATRIX waveNormalTransform0;
	DirectX::XMMATRIX waveNormalTransform1;

	waveDispOffset0.x += 0.01f * mTotalTime;
	waveDispOffset0.y += 0.03f * mTotalTime;

	waveDispOffset1.x += 0.01f * mTotalTime;
	waveDispOffset1.y += 0.03f * mTotalTime;

	DirectX::XMMATRIX waveScale0 = DirectX::XMMatrixScaling(2.0f, 2.0f, 1.0f);
	DirectX::XMMATRIX waveOffset0 = DirectX::XMMatrixTranslation(waveDispOffset0.x, waveDispOffset0.y, 0.0f);
	waveDispTexTransform0 = DirectX::XMMatrixMultiply(waveScale0, waveOffset0);

	DirectX::XMMATRIX waveScale1 = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	DirectX::XMMATRIX waveOffset1 = DirectX::XMMatrixTranslation(waveDispOffset1.x, waveDispOffset1.y, 0.0f);
	waveDispTexTransform1 = DirectX::XMMatrixMultiply(waveScale1, waveOffset1);

	waveNormalOffset0.x += 0.05f * mTotalTime;
	waveNormalOffset0.y += 0.2f * mTotalTime;

	waveNormalOffset1.x += 0.02f * mTotalTime;
	waveNormalOffset1.y += 0.05f * mTotalTime;

	waveScale0 = DirectX::XMMatrixScaling(22.0f, 22.0f, 1.0f);
	waveOffset0 = DirectX::XMMatrixTranslation(waveNormalOffset0.x, waveNormalOffset0.y, 0.0f);
	waveNormalTransform0 = DirectX::XMMatrixMultiply(waveScale0, waveOffset0);

	waveScale1 = DirectX::XMMatrixScaling(16.0f, 16.0f, 1.0f);
	waveOffset1 = DirectX::XMMatrixTranslation(waveNormalOffset1.x, waveNormalOffset1.y, 0.0f);
	waveNormalTransform1 = DirectX::XMMatrixMultiply(waveScale1, waveOffset1);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(pgfx_pDeviceContext->Map(constBuffersMap.at(cbNames.tessWavesMatrices), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	cbVSTesselationWaves* pVSCB = reinterpret_cast<cbVSTesselationWaves*>(mappedData.pData);
	pVSCB->cameraPosition = mCameraPosition;
	pVSCB->texTransform = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	pVSCB->world = DirectX::XMMatrixTranspose(world);
	pVSCB->worldInvTranspose = MathHelper::InverseTranspose(world);
	pVSCB->worldViewProjection = DirectX::XMMatrixTranspose(world * mViewProjection);
	pVSCB->waveDispTexTransform0 = DirectX::XMMatrixTranspose(waveDispTexTransform0);
	pVSCB->waveDispTexTransform1 = DirectX::XMMatrixTranspose(waveDispTexTransform1);
	pVSCB->waveNormalTexTransform0 = DirectX::XMMatrixTranspose(waveNormalTransform0);
	pVSCB->waveNormalTexTransform1 = DirectX::XMMatrixTranspose(waveNormalTransform1);
	pgfx_pDeviceContext->Unmap(constBuffersMap.at(cbNames.tessWavesMatrices), 0u);


	DX::ThrowIfFailed(pgfx_pDeviceContext->Map(constBuffersMap.at(cbNames.defaultLightPerFrame), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	cbDefaultLightPSPerFrame* psBuff = reinterpret_cast<cbDefaultLightPSPerFrame*> (mappedData.pData);
	psBuff->camPositon = mCameraPosition;
	psBuff->disableTexSampling = false;
	psBuff->lightDirection = mNewLightDirection;
	psBuff->mat = mat;
	psBuff->useSSAO = false;
	pgfx_pDeviceContext->Unmap(constBuffersMap.at(cbNames.defaultLightPerFrame), 0u);


	DX::ThrowIfFailed(pgfx_pDeviceContext->Map(constBuffersMap.at(cbNames.defaultVS), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	cbDefaultMatricesVS* frameDS = reinterpret_cast<cbDefaultMatricesVS*> (mappedData.pData);
	frameDS->cameraPosition = mCameraPosition;
	frameDS->viewProjection = DirectX::XMMatrixTranspose(mViewProjection);
	pgfx_pDeviceContext->Unmap(constBuffersMap.at(cbNames.defaultVS), 0u);

}


void Graphics::SetTerrainShaderResAndUpdateCbuffers(const DirectX::XMMATRIX world,
	const std::wstring blendMap, const std::wstring snowMap,
	ID3D11ShaderResourceView* pHeightMapDS,
	ID3D11ShaderResourceView* pHeightMapVS)
{
   
	DirectX::XMFLOAT4 worldPlanes[6];
	ExtractFrustumPlanes(worldPlanes, world * mViewProjection);

	pgfx_pDeviceContext->VSSetShaderResources(0u, 1u, &pHeightMapVS);

	pgfx_pDeviceContext->DSSetShaderResources(0u, 1u, &pHeightMapDS);

	pgfx_pDeviceContext->PSSetShaderResources(2u, 1u, &diffuseMaps.at(blendMap));
	pgfx_pDeviceContext->PSSetShaderResources(3u, 1u, &diffuseMaps.at(snowMap));

	pgfx_pDeviceContext->HSSetConstantBuffers(0u, 1u, &constBuffersMap.at(cbNames.terrainHSPlainsData));
	pgfx_pDeviceContext->DSSetConstantBuffers(0u, 1u, &constBuffersMap.at(cbNames.defaultVS));
	pgfx_pDeviceContext->PSSetConstantBuffers(2u, 1u, &constBuffersMap.at(cbNames.terrainTexelInfo));
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(pgfx_pDeviceContext->Map(constBuffersMap.at(cbNames.terrainHSPlainsData), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	cbHSTerrainPerFrame* pHullShader = reinterpret_cast<cbHSTerrainPerFrame*>(mappedData.pData);
	pHullShader->cameraPosition = mCameraPosition;
	pHullShader->worldFrustumPlanes[0] = worldPlanes[0];
	pHullShader->worldFrustumPlanes[1] = worldPlanes[1];
	pHullShader->worldFrustumPlanes[2] = worldPlanes[2];
	pHullShader->worldFrustumPlanes[3] = worldPlanes[3];
	pHullShader->worldFrustumPlanes[4] = worldPlanes[4];
	pHullShader->worldFrustumPlanes[5] = worldPlanes[5];
	pgfx_pDeviceContext->Unmap(constBuffersMap.at(cbNames.terrainHSPlainsData), 0u);


	DX::ThrowIfFailed(pgfx_pDeviceContext->Map(constBuffersMap.at(cbNames.defaultVS), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	cbDefaultMatricesVS* pDomainShader = reinterpret_cast<cbDefaultMatricesVS*>(mappedData.pData);
	pDomainShader->viewProjection = DirectX::XMMatrixTranspose(mViewProjection);
	pDomainShader->world = DirectX::XMMatrixTranspose(world);
	pgfx_pDeviceContext->Unmap(constBuffersMap.at(cbNames.defaultVS), 0u);
}

void Graphics::BindCubeMap(std::wstring& skyBoxName) const
{
	pgfx_pDeviceContext->PSSetShaderResources(4u, 1u, &cubeMaps.at(skyBoxName));
}




void Graphics::DrawParticle(DirectX::XMFLOAT3& emitPos, ParticlePick particle)
{
	const float blendFactorsZero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (GetAsyncKeyState('4') & 0x8000)
	{
		if (mTotalTime - lastResetTime > 1.0f)
		{
			mfirstRunRain = true;
			mfirstRunExplosion = true;
			lastResetTime = mTotalTime;
		}
	}

	switch (particle)
	{
	case ParticlePick::Fire:
	{
		pgfx_pDeviceContext->OMSetBlendState(additiveBlend, blendFactorsZero, 0xffffffff);
		pgfx_pDeviceContext->OMSetDepthStencilState(disableDepthWrites, 0u);
		BindVSandIA(ShaderPicker::Particles_FireStreamOut_VS_GS);
		BindGS(ShaderPicker::Particles_FireStreamOut_VS_GS);
		break;
	}
	case ParticlePick::Rain:
	{
		BindVSandIA(ShaderPicker::Particles_RainStreamOut_VS_GS);
		BindGS(ShaderPicker::Particles_RainStreamOut_VS_GS);
		break;
	}
	case ParticlePick::Fountain:
	{
		BindVSandIA(ShaderPicker::Particle_FountainStreamOut_VS_GS);
		BindGS(ShaderPicker::Particle_FountainStreamOut_VS_GS);
		break;
	}
	case ParticlePick::Explosion:
	{
		pgfx_pDeviceContext->OMSetBlendState(additiveBlend, blendFactorsZero, 0xffffffff);
		pgfx_pDeviceContext->OMSetDepthStencilState(disableDepthWrites, 0u);
		BindVSandIA(ShaderPicker::Particles_ExplosionStreamOut_VS_GS);
		BindGS(ShaderPicker::Particles_ExplosionStreamOut_VS_GS);
		break;
	}

	default:
		break;
	}

	UINT stride = sizeof(Particle);
	UINT offset = 0;
	switch (particle)
	{
	case ParticlePick::Fire:
	{
		if (mFirstRunFire)
		{
		BindToSOStage(mStreamOutVBFire);
		UpdateStreamOutConstBuffer(emitPos);
		pgfx_pDeviceContext->IASetVertexBuffers(0u, 1u, &mInitVBFire, &stride, &offset);
		}
		else
		{
		BindToSOStage(mStreamOutVBFire);
		UpdateStreamOutConstBuffer(emitPos);
		pgfx_pDeviceContext->IASetVertexBuffers(0u, 1u, &mDrawVBFire, &stride, &offset);
		}
	pgfx_pDeviceContext->SOSetTargets(1u, &mStreamOutVBFire, &offset);
	break;
	}
	case ParticlePick::Rain:
	{
		if (mfirstRunRain)
		{
			BindToSOStage(mStreamOutVBRain);
			UpdateStreamOutConstBuffer(emitPos);
			pgfx_pDeviceContext->IASetVertexBuffers(0u, 1u, &mInitVBRain, &stride, &offset);
		}
		else
		{
			BindToSOStage(mStreamOutVBRain);
			UpdateStreamOutConstBuffer(emitPos);
			pgfx_pDeviceContext->IASetVertexBuffers(0u, 1u, &mDrawVBRain, &stride, &offset);
		}

		pgfx_pDeviceContext->SOSetTargets(1u, &mStreamOutVBRain, &offset);

		break;
	}
	case ParticlePick::Explosion:
	{
		if (mfirstRunExplosion)
		{
			BindToSOStage(mStreamOutVBExplosion);
			UpdateStreamOutConstBuffer(emitPos);
			pgfx_pDeviceContext->IASetVertexBuffers(0u, 1u, &mInitVBExplosion, &stride, &offset);
		}
		else
		{
			BindToSOStage(mStreamOutVBExplosion);
			UpdateStreamOutConstBuffer(emitPos);
			pgfx_pDeviceContext->IASetVertexBuffers(0u, 1u, &mDrawVBExplosion, &stride, &offset);
		}

		pgfx_pDeviceContext->SOSetTargets(1u, &mStreamOutVBExplosion, &offset);

		break;
	}
	case ParticlePick::Fountain:
	{
		if (mFirstRunFountain)
		{
		BindToSOStage(mStreamOutVBFountain);
		UpdateStreamOutConstBuffer(emitPos);
		pgfx_pDeviceContext->IASetVertexBuffers(0u, 1u, &mInitVBFountain, &stride, &offset);
		}
		else
		{
		BindToSOStage(mStreamOutVBFountain);
		UpdateStreamOutConstBuffer(emitPos);
		pgfx_pDeviceContext->IASetVertexBuffers(0u, 1u, &mDrawVBFountain, &stride, &offset);
		}
	pgfx_pDeviceContext->SOSetTargets(1u, &mStreamOutVBFountain, &offset);
	break;
	}
		break;
	default:
		break;
	}
// 	if (mfirstRun)
// 	{
// 		BindToSOStage(mStreamOutVB);
// 		UpdateStreamOutConstBuffer(emitPos);
// 		pgfx_pDeviceContext->IASetVertexBuffers(0u, 1u, &mInitVB, &stride, &offset);
// 	}
// 	else
// 	{
// 		BindToSOStage(mStreamOutVB);
// 		UpdateStreamOutConstBuffer(emitPos);
// 		pgfx_pDeviceContext->IASetVertexBuffers(0u, 1u, &mDrawVB, &stride, &offset);
// 	}
// 
// 	pgfx_pDeviceContext->SOSetTargets(1u, &mStreamOutVB, &offset);

	switch (particle)
	{
	case ParticlePick::Fire:
	{
		if (mFirstRunFire)
		{
			pgfx_pDeviceContext->Draw(1u, 0u);
			mFirstRunFire = false;
		}
		else
		{
			pgfx_pDeviceContext->DrawAuto();
		}
		break;
	}
	case ParticlePick::Rain:
	{
		if (mfirstRunRain)
		{
			pgfx_pDeviceContext->Draw(1u, 0u);
			mfirstRunRain = false;
		}
		else
		{
			pgfx_pDeviceContext->DrawAuto();
		}
		break;
	}
	case ParticlePick::Explosion:
	{
		if (mfirstRunExplosion)
		{
			pgfx_pDeviceContext->Draw(1u, 0u);
			mfirstRunExplosion = false;
		}
		else
		{
			pgfx_pDeviceContext->DrawAuto();
		}
	}
		break;
	case ParticlePick::Fountain:
	{
		if (mFirstRunFountain)
		{
			pgfx_pDeviceContext->Draw(1u, 0u);
			mFirstRunFountain = false;
		}
		else
		{
			pgfx_pDeviceContext->DrawAuto();
		}
	}
		break;
	default:
		break;
	}

	//done streaming-out--unbind the vertex buffer
	UnbindFromSOStage();
	switch (particle)
	{
	case ParticlePick::Fire:
	{
		std::swap(mDrawVBFire, mStreamOutVBFire);
		pgfx_pDeviceContext->IASetVertexBuffers(0, 1, &mDrawVBFire, &stride, &offset);
		break;
	}
	case ParticlePick::Rain:
	{
		std::swap(mDrawVBRain, mStreamOutVBRain);
		pgfx_pDeviceContext->IASetVertexBuffers(0, 1, &mDrawVBRain, &stride, &offset);
		break;
	}
	case ParticlePick::Explosion:
	{
		std::swap(mDrawVBExplosion, mStreamOutVBExplosion);
		pgfx_pDeviceContext->IASetVertexBuffers(0, 1, &mDrawVBExplosion, &stride, &offset);
		break;
	}
	case ParticlePick::Fountain:
	{
		std::swap(mDrawVBFountain, mStreamOutVBFountain);
		pgfx_pDeviceContext->IASetVertexBuffers(0, 1, &mDrawVBFountain, &stride, &offset);
		break;
	}
	default:
		break;
	}
// 	std::swap(mDrawVB, mStreamOutVB);
// 	pgfx_pDeviceContext->IASetVertexBuffers(0, 1, &mDrawVB, &stride, &offset);

	UpdateParticleDrawConstBuffer();

	// Draw the updated particle system we just streamed-out.
	switch (particle)
	{
	case ParticlePick::Fire:
	{
		BindVSandIA(ShaderPicker::Particles_FireDraw_VS_GS_PS);
		BindGS(ShaderPicker::Particles_FireDraw_VS_GS_PS);
		BindPS(ShaderPicker::Particles_FireDraw_VS_GS_PS);
		pgfx_pDeviceContext->PSSetShaderResources(0u, 1u, &diffuseMaps.at(L"flame"));

	}
	break;
	case ParticlePick::Rain:
	{
		BindVSandIA(ShaderPicker::Particles_RainDraw_VS_GS_PS);
		BindGS(ShaderPicker::Particles_RainDraw_VS_GS_PS);
		BindPS(ShaderPicker::Particles_RainDraw_VS_GS_PS);
		pgfx_pDeviceContext->PSSetShaderResources(0u, 1u, &diffuseMaps.at(L"raindrop"));

		break;
	}
	case ParticlePick::Fountain:
	{
		BindVSandIA(ShaderPicker::Particle_FountainDraw_VS_GS_PS);
		BindGS(ShaderPicker::Particle_FountainDraw_VS_GS_PS);
		BindPS(ShaderPicker::Particle_FountainDraw_VS_GS_PS);
		pgfx_pDeviceContext->PSSetShaderResources(0u, 1u, &diffuseMaps.at(L"raindrop"));
		break;
	}
	case ParticlePick::Explosion:
	{
		BindVSandIA(ShaderPicker::Particle_ExplosionDraw_VS_GS_PS);
		BindGS(ShaderPicker::Particle_ExplosionDraw_VS_GS_PS);
		BindPS(ShaderPicker::Particle_ExplosionDraw_VS_GS_PS);
		pgfx_pDeviceContext->PSSetShaderResources(0u, 1u, &diffuseMaps.at(L"flame"));
	}
	break;
	default:
		break;
	}


	pgfx_pDeviceContext->DrawAuto();

}


void Graphics::SetParticleBuffers(ID3D11Buffer* pStreamOutVB, ID3D11Buffer* pDrawVB, ID3D11ShaderResourceView* randomTexSRV, ID3D11Buffer* pInitVB,
	ParticlePick particle)
{
	switch (particle)
	{
	case ParticlePick::Fire:
	{
		mStreamOutVBFire = pStreamOutVB;
		mDrawVBFire = pDrawVB;
		mInitVBFire = pInitVB;
		break;
	}
	case ParticlePick::Rain:
	{
		mStreamOutVBRain = pStreamOutVB;
		mDrawVBRain = pDrawVB;
		mInitVBRain = pInitVB;

		break;
	}
	case ParticlePick::Explosion:
	{
		mStreamOutVBExplosion = pStreamOutVB;
		mDrawVBExplosion = pDrawVB;
		mInitVBExplosion = pInitVB;
		break;
	}
	case ParticlePick::Fountain:
	{
		mStreamOutVBFountain = pStreamOutVB;
		mDrawVBFountain = pDrawVB;
		mInitVBFountain = pInitVB;
		break;
	}
	default:
		break;
	}
	pgfx_pDeviceContext->GSSetShaderResources(0u, 1u, &randomTexSRV);

}

void Graphics::CreateM3dModel(M3dRawData& data, const std::string& name)
{
	M3dModel model;
	model.pVertexBuffer = CreateVertexBuffer(data.vertices, false, false, L"Temple Base vertices");
	model.pIndexBuffer = CreateIndexBuffer(data.indices, L"temple base indices");
	MaterialM3d m;
	for (auto& a : data.mats)
	{
		m.mat.diffuseAlbedo = a.diffuseAlbedo;
		m.mat.fresnelR0 = a.fresnelR0;
		m.mat.shininess = a.shininess;
		m.name = a.materialTypeName;
		m.diffuseMapName = a.diffuseMapName;
		m.normalMapName = a.normalMapName;
		model.mats.push_back(m);
	}
	model.subsets.resize(data.subsets.size());
	model.subsets = data.subsets;
	m3dModelsMap.insert(std::make_pair(name, model));
	for (size_t i = 0; i < data.mats.size(); i++)
	{
		ID3D11ShaderResourceView* pTemp = nullptr;
		std::wstring path = L"Textures\\";
		path += data.mats[i].diffuseMapName + L".dds";
		pTemp = CreateSRV(path, false);
		diffuseMaps.insert(std::make_pair(data.mats[i].diffuseMapName, pTemp));
	}
	for (size_t i = 0; i < data.mats.size(); i++)
	{
		ID3D11ShaderResourceView* pTemp = nullptr;
		std::wstring path = L"Textures\\";
		path += data.mats[i].normalMapName + L".dds";
		pTemp = CreateSRV(path, false);
		normalMaps.insert(std::make_pair(data.mats[i].normalMapName, pTemp));
	}

}

void Graphics::CreateM3dModel(M3dRawSkinnedData& data, const std::string& name)
{
	M3dSkinnedModel model;
	model.pVertexBuffer = CreateVertexBuffer(data.vertices, false, false, L"Temple Base vertices");
	model.pIndexBuffer = CreateIndexBuffer(data.indices, L"temple base indices");
	MaterialM3d m;
	for (auto& a : data.mats)
	{
		m.mat.diffuseAlbedo = a.diffuseAlbedo;
		m.mat.fresnelR0 = a.fresnelR0;
		m.mat.shininess = a.shininess;
		m.name = a.materialTypeName;
		m.diffuseMapName = a.diffuseMapName;
		m.normalMapName = a.normalMapName;
		model.mats.push_back(m);
	}
	model.subsets.resize(data.subsets.size());
	model.subsets = data.subsets;
	m3dSkinnedModelMap.insert(std::make_pair(name, model));
	for (size_t i = 0; i < data.mats.size(); i++)
	{
		ID3D11ShaderResourceView* pTemp = nullptr;
		std::wstring path = L"Textures\\";
		path += data.mats[i].diffuseMapName + L".dds";
		pTemp = CreateSRV(path, false);
		diffuseMaps.insert(std::make_pair(data.mats[i].diffuseMapName, pTemp));
	}
	for (size_t i = 0; i < data.mats.size(); i++)
	{
		ID3D11ShaderResourceView* pTemp = nullptr;
		std::wstring path = L"Textures\\";
		path += data.mats[i].normalMapName + L".dds";
		pTemp = CreateSRV(path, false);
		normalMaps.insert(std::make_pair(data.mats[i].normalMapName, pTemp));
	}
	model.mBoneHierarchy.resize(data.skinnedInfo.mBoneHierarchy.size());
	model.mBoneHierarchy = data.skinnedInfo.mBoneHierarchy;
	model.mBoneOffsets.resize(data.skinnedInfo.mBoneOffsets.size());
	model.mBoneOffsets = data.skinnedInfo.mBoneOffsets;
}

void Graphics::DrawM3dStaticModel(std::string name, Technique tech, std::vector<DirectX::XMMATRIX> world)
{
	bool usessao = true;
	if (GetAsyncKeyState('5') & 0x8000)
		usessao = false;
	else
		usessao = true;

	UINT stride = sizeof(vbPosNormalTexTangent);
	UINT offset = 0;
	M3dModel model = m3dModelsMap.at(name);
	pgfx_pDeviceContext->IASetVertexBuffers(0u, 1u, &model.pVertexBuffer, &stride, &offset);
	pgfx_pDeviceContext->IASetIndexBuffer(model.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	for (size_t w = 0; w < world.size(); w++)
	{
		for (size_t i = 0; i < model.subsets.size(); i++)
		{
			MaterialEx mat;
			mat.diffuseAlbedo = model.mats[i].mat.diffuseAlbedo;
			mat.fresnelR0 = model.mats[i].mat.fresnelR0;
			mat.shininess = model.mats[i].mat.shininess;
			switch (tech)
			{
			case Technique::NormalMap:
			{
				NormalMap(world[w]);
				break;
			}
			case Technique::DefaultLight:
			{
				DefaultLightUpdate(mat, false, usessao, model.mats[i].diffuseMapName, model.mats[i].normalMapName);
				break;
			}
			case Technique::ShadowMap:
			{
				ShadowMap(world[w], mLightViewProjection);
				break;
			}
			default:
				break;
			}
			VSDefaultMatricesUpdate(world[w], DirectX::XMMatrixIdentity(), DirectX::XMMatrixIdentity());
			pgfx_pDeviceContext->DrawIndexed(model.subsets[i].FaceCount * 3, model.subsets[i].FaceStart * 3, 0);
		}
	}
}

void Graphics::SetComputeWavesResources()
{
	pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &constBuffersMap.at(cbNames.defaultVS));
	pgfx_pDeviceContext->VSSetConstantBuffers(1u, 1u, &constBuffersMap.at(cbNames.computeWavesVSData));


	pgfx_pDeviceContext->CSSetConstantBuffers(0u, 1u, &constBuffersMap.at(cbNames.computeWavesCSPerFrame));
}

void Graphics::UpdateComputeWaves(const DirectX::XMMATRIX& in_world)
{
	waterTextureOffset.y += 0.05f * mDeltaTime;
	waterTextureOffset.x += 0.1f * mDeltaTime;
	wavesOffset = DirectX::XMMatrixTranslation(0.0f, waterTextureOffset.x, waterTextureOffset.y);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(pgfx_pDeviceContext->Map(constBuffersMap.at(cbNames.defaultVS), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	cbDefaultMatricesVS* object = reinterpret_cast<cbDefaultMatricesVS*>(mappedData.pData);
	object->world = in_world;
	object->worldInvTranspose = MathHelper::InverseTranspose(in_world);
	object->texTransform = DirectX::XMMatrixTranspose(wavesOffset + wavesScale);
	object->viewProjection = DirectX::XMMatrixTranspose(mViewProjection);
	pgfx_pDeviceContext->Unmap(constBuffersMap.at(cbNames.defaultVS), 0u);
}

void Graphics::DisturbComputeWaves(const UINT numColumns, const UINT numRows, DirectX::XMFLOAT3 waveContants)
{
	unsigned long i = 5 + MathHelper::RandomIntWithingRange(0, INT_MAX) % (numColumns - 10);
	unsigned long j = 5 + MathHelper::RandomIntWithingRange(0, INT_MAX) % (numRows - 10);
	float magnitute = MathHelper::RandomFloatWithinRange(1.0f, 2.0f);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(pgfx_pDeviceContext->Map(constBuffersMap.at(cbNames.computeWavesCSPerFrame), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	cbWavesUpdateCS* data = reinterpret_cast<cbWavesUpdateCS*> (mappedData.pData);
	data->disturbIndex[0] = i;
	data->disturbIndex[1] = j;
	data->disturbMagnitute = magnitute;
	data->waveConstant0 = waveContants.x;
	data->waveConstant1 = waveContants.y;
	data->waveConstant2 = waveContants.z;
	pgfx_pDeviceContext->Unmap(constBuffersMap.at(cbNames.computeWavesCSPerFrame), 0u);

	pgfx_pDeviceContext->CSSetUnorderedAccessViews(0u, 1u, &pCurrentSolutionUAV, 0u);
	pgfx_pDeviceContext->Dispatch(1, 1, 1);

	ID3D11UnorderedAccessView* nullUAV = nullptr;
	pgfx_pDeviceContext->CSSetUnorderedAccessViews(0u, 1u, &nullUAV, 0u);
}

void Graphics::UpdateSolutionComputeWaves(const UINT numColumns, const UINT numRows)
{
	time += mDeltaTime;
	if (time >= mDeltaTime)
	{
		pgfx_pDeviceContext->CSSetShaderResources(0u, 1u, &pPreviousSolutionSRV);
		pgfx_pDeviceContext->CSSetShaderResources(1u, 1u, &pCurrentSolutionSRV);
		pgfx_pDeviceContext->CSSetUnorderedAccessViews(0u, 1u, &pNextSolutionUAV, 0u);

		UINT numGroupsX = numColumns / 16;
		UINT numGroupsY = numRows / 16;
		pgfx_pDeviceContext->Dispatch(numGroupsX, numGroupsY, 1);

		ID3D11UnorderedAccessView* nullUAV = nullptr;
		ID3D11ShaderResourceView* nullSRV = nullptr;
		pgfx_pDeviceContext->CSSetShaderResources(0u, 1u, &nullSRV);
		pgfx_pDeviceContext->CSSetShaderResources(1u, 1u, &nullSRV);
		pgfx_pDeviceContext->CSSetUnorderedAccessViews(0u, 1u, &nullUAV, 0u);

		ID3D11ShaderResourceView* srvTemp = pPreviousSolutionSRV;
		pPreviousSolutionSRV = pCurrentSolutionSRV;
		pCurrentSolutionSRV = pNextSolutionSRV;
		pNextSolutionSRV = srvTemp;

		ID3D11UnorderedAccessView* uavTemp = pPreviousSolutionUAV;
		pPreviousSolutionUAV = pCurrentSolutionUAV;
		pCurrentSolutionUAV = pNextSolutionUAV;
		pNextSolutionUAV = uavTemp;

		time = 0.0f;
	}
	pgfx_pDeviceContext->VSSetShaderResources(0u, 1u, &pCurrentSolutionSRV);

}

void Graphics::ComputeWavesClearVS()
{
	ID3D11ShaderResourceView* nullSRV = nullptr;
	pgfx_pDeviceContext->VSSetShaderResources(0u, 1u, &nullSRV);
}

void Graphics::SetWavesCSResources(ID3D11ShaderResourceView* prevSol, ID3D11ShaderResourceView* currSol,
	ID3D11ShaderResourceView* nextSol, ID3D11UnorderedAccessView* prevSolUA,
	ID3D11UnorderedAccessView* currSolUA, ID3D11UnorderedAccessView* nexSolUA)
{
	pPreviousSolutionUAV = prevSolUA;
	pCurrentSolutionUAV = currSolUA;
	pNextSolutionUAV = nexSolUA;

	pPreviousSolutionSRV = prevSol;
	pCurrentSolutionSRV = currSol;
	pNextSolutionSRV = nextSol;
}

void Graphics::SetComputeWavesSamplers()
{
	pgfx_pDeviceContext->VSSetSamplers(4u, 1u, &samplers[4]);



}

void Graphics::BindToSOStage(ID3D11Buffer* pStreamOutVB)
{
	pgfx_pDeviceContext->SOSetTargets(1u, &pStreamOutVB, 0u);
}

void Graphics::UnbindFromSOStage()
{
	ID3D11Buffer* bufferArray[1] = { 0 };
	pgfx_pDeviceContext->SOSetTargets(1u, bufferArray, 0u);
}

void Graphics::UpdateStreamOutConstBuffer(DirectX::XMFLOAT3& emitPos)
{
	pgfx_pDeviceContext->GSSetConstantBuffers(0u, 1u, &constBuffersMap.at(cbNames.particleStreamOutGS));

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(pgfx_pDeviceContext->Map(constBuffersMap.at(cbNames.particleStreamOutGS), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	cbParticleStreamOutGS* StreamOut = reinterpret_cast<cbParticleStreamOutGS*>(mappedData.pData);
	StreamOut->emitterPositon = emitPos;
	StreamOut->gameTime = mTotalTime;
	StreamOut->timeStep = mDeltaTime;
	pgfx_pDeviceContext->Unmap(constBuffersMap.at(cbNames.particleStreamOutGS), 0u);

}

void Graphics::UpdateParticleDrawConstBuffer()
{
	pgfx_pDeviceContext->GSSetConstantBuffers(0u, 1u, &constBuffersMap.at(cbNames.defaultVS));

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(pgfx_pDeviceContext->Map(constBuffersMap.at(cbNames.defaultVS), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	cbDefaultMatricesVS* drawParticleGS = reinterpret_cast<cbDefaultMatricesVS*>(mappedData.pData);
	drawParticleGS->cameraPosition = mCameraPosition;
	drawParticleGS->viewProjection = DirectX::XMMatrixTranspose(mViewProjection);
	pgfx_pDeviceContext->Unmap(constBuffersMap.at(cbNames.defaultVS), 0u);

}

ID3D11Buffer* Graphics::CreateIndexBuffer(const std::vector<UINT> indices, const std::wstring& name)
{
	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.ByteWidth = (sizeof(UINT) * static_cast<UINT>(indices.size()));
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


void Graphics::CreateSRVArray(ID3D11ShaderResourceView** pSRV, UINT nImages, std::wstring* in_path)
{
	DirectX::TexMetadata textureMetaData;
	std::vector<DirectX::Image> ImagesArray;

	for (size_t i = 0; i < nImages; ++i)
	{
		DirectX::ScratchImage* pImageData = new DirectX::ScratchImage();
		LoadFromDDSFile(in_path[i].c_str(), DirectX::DDS_FLAGS_NONE, &textureMetaData, *pImageData);
		for (size_t mip = 0; mip < textureMetaData.mipLevels; ++mip)
		{
			const DirectX::Image* image = pImageData->GetImage(mip, 0, 0);
			DirectX::Image toPush;
			toPush.format = textureMetaData.format;
			toPush.height = textureMetaData.height;
			toPush.rowPitch = image->rowPitch;
			toPush.slicePitch = image->slicePitch;
			toPush.width = textureMetaData.width;
			toPush.pixels = image->pixels;
			ImagesArray.push_back(toPush);
		}
	}
	textureMetaData.arraySize = nImages;
	DirectX::CreateShaderResourceView(pgfx_pDevice.Get(), ImagesArray.data(),
		nImages * textureMetaData.mipLevels,
		textureMetaData, pSRV);

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

}

void Graphics::BlurSSAOMap(ID3D11ShaderResourceView* pInputSRV, ID3D11RenderTargetView* pOutputRTV,
	D3D11_VIEWPORT ssaoViewPort, bool horizontalBlur)
{
	ID3D11RenderTargetView* renderTargets[1] = { pOutputRTV };
	pgfx_pDeviceContext->OMSetRenderTargets(1, &renderTargets[0], 0);
	pgfx_pDeviceContext->ClearRenderTargetView(renderTargets[0], DirectX::Colors::Black);
	pgfx_pDeviceContext->RSSetViewports(1u, &ssaoViewPort);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(pgfx_pDeviceContext->Map(constBuffersMap.at(cbNames.ssaoBlur), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
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
	pgfx_pDeviceContext->Unmap(constBuffersMap.at(cbNames.ssaoBlur), 0u);

	pgfx_pDeviceContext->PSSetConstantBuffers(0u, 1u, &constBuffersMap.at(cbNames.ssaoBlur));
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

void Graphics::ReleaseSSAOShaderResource()
{
	ID3D11ShaderResourceView* pNullSRV = nullptr;
	pgfx_pDeviceContext->PSSetShaderResources(5u, 1u, &pNullSRV);
}

void Graphics::DefaultLightUpdate(MaterialEx& mat, BOOL disableTexSamling, BOOL useSSAO,
	const std::wstring& diffuseMap, const std::wstring& normalMap)
{

	BindDiffuseMap(diffuseMap);
	BindNormalMap(normalMap);
	D3D11_MAPPED_SUBRESOURCE mappedData;
	pgfx_pDeviceContext->Map(constBuffersMap.at(cbNames.defaultLightPerFrame), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData);
	cbDefaultLightPSPerFrame* pBuffer = reinterpret_cast<cbDefaultLightPSPerFrame*>(mappedData.pData);
	pBuffer->camPositon = mCameraPosition;
	pBuffer->disableTexSampling = disableTexSamling;
	pBuffer->lightDirection = mNewLightDirection;
	pBuffer->mat = mat;
	pBuffer->useSSAO = useSSAO;
	pgfx_pDeviceContext->Unmap(constBuffersMap.at(cbNames.defaultLightPerFrame), 0u);

}

void Graphics::TerrainLightUpdate(MaterialEx& mat, BOOL disableTexSamling, BOOL useSSAO)
{
	pgfx_pDeviceContext->PSSetShaderResources(0u, 1u, diffuseMapArray.at(L"TerrainLayerMaps"));
	pgfx_pDeviceContext->PSSetShaderResources(1u, 1u, &normalMaps.at(L"TerrainHeightMap"));
	D3D11_MAPPED_SUBRESOURCE mappedData;
	pgfx_pDeviceContext->Map(constBuffersMap.at(cbNames.defaultLightPerFrame), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData);
	cbDefaultLightPSPerFrame* pBuffer = reinterpret_cast<cbDefaultLightPSPerFrame*>(mappedData.pData);
	pBuffer->camPositon = mCameraPosition;
	pBuffer->disableTexSampling = disableTexSamling;
	pBuffer->lightDirection = mDefaultLightDirection;
	pBuffer->mat = mat;
	pBuffer->useSSAO = useSSAO;
	pgfx_pDeviceContext->Unmap(constBuffersMap.at(cbNames.defaultLightPerFrame), 0u);
}

void Graphics::SetDefaultLightData()
{
	pgfx_pDeviceContext->PSSetConstantBuffers(0u, 1u, &constBuffersMap.at(cbNames.defaultLightData));
	pgfx_pDeviceContext->PSSetConstantBuffers(1u, 1u, &constBuffersMap.at(cbNames.defaultLightPerFrame));

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

void Graphics::BindDiffuseMap(const std::wstring& diffMapName) const
{
	pgfx_pDeviceContext->PSSetShaderResources(0u, 1u, &diffuseMaps.at(diffMapName));
}

void Graphics::BindNormalMap(const std::wstring& normalMapName) const
{
	pgfx_pDeviceContext->PSSetShaderResources(1u, 1u, &normalMaps.at(normalMapName));
}

void Graphics::ExtractFrustumPlanes(DirectX::XMFLOAT4 planes[6], DirectX::CXMMATRIX _M)
{
	using namespace DirectX;
	//
	// Left
	//
	DirectX::XMFLOAT4X4 M;
	DirectX::XMStoreFloat4x4(&M, _M);

	planes[0].x = M(0, 3) + M(0, 0);
	planes[0].y = M(1, 3) + M(1, 0);
	planes[0].z = M(2, 3) + M(2, 0);
	planes[0].w = M(3, 3) + M(3, 0);

	//
	// Right
	//
	planes[1].x = M(0, 3) - M(0, 0);
	planes[1].y = M(1, 3) - M(1, 0);
	planes[1].z = M(2, 3) - M(2, 0);
	planes[1].w = M(3, 3) - M(3, 0);

	//
	// Bottom
	//
	planes[2].x = M(0, 3) + M(0, 1);
	planes[2].y = M(1, 3) + M(1, 1);
	planes[2].z = M(2, 3) + M(2, 1);
	planes[2].w = M(3, 3) + M(3, 1);

	//
	// Top
	//
	planes[3].x = M(0, 3) - M(0, 1);
	planes[3].y = M(1, 3) - M(1, 1);
	planes[3].z = M(2, 3) - M(2, 1);
	planes[3].w = M(3, 3) - M(3, 1);

	//
	// Near
	//
	planes[4].x = M(0, 2);
	planes[4].y = M(1, 2);
	planes[4].z = M(2, 2);
	planes[4].w = M(3, 2);

	//
	// Far
	//
	planes[5].x = M(0, 3) - M(0, 2);
	planes[5].y = M(1, 3) - M(1, 2);
	planes[5].z = M(2, 3) - M(2, 2);
	planes[5].w = M(3, 3) - M(3, 2);

	// Normalize the plane equations.
	for (int i = 0; i < 6; ++i)
	{
		DirectX::XMVECTOR v = DirectX::XMPlaneNormalize(DirectX::XMLoadFloat4(&planes[i]));
		DirectX::XMStoreFloat4(&planes[i], v);
	}
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

	//bind all samplers to PS once and at the creation
	for (int i = 0; i < samplers.size(); i++)
	{
		pgfx_pDeviceContext->PSSetSamplers(i, 1u, &samplers[i]);
	}
	//tessellation waves
	pgfx_pDeviceContext->DSSetSamplers(0u, 1u, &samplers[1]);
	//terrain height map
	pgfx_pDeviceContext->VSSetSamplers(0u, 1u, &samplers[0]);
	//particles GS stream out
	pgfx_pDeviceContext->GSSetSamplers(0u, 1u, &samplers[0]);
}




void Graphics::InitShaders()
{
	VS_IL_Init(&pSkyVS, IL.positonIL, &pSkyIL, IL.nPosition, L"Shaders\\Vertex\\SkyVS.cso");
	PS_Init(&pSkyPS, L"Shaders\\Pixel\\SkyPS.cso");

	//SSAO
	//compute ssao
	VS_IL_Init(&pSSAOFullScreenQuadVS, IL.posNormalTexture, &pPosNormalTexIL,
		IL.nPosNormalTexture, L"Shaders\\Vertex\\ComputeSSAOVS.cso");
	PS_Init(&pSSAOFullScreenQuadPS, L"Shaders\\Pixel\\ComputeSSAOPS.cso");
	//SSAO blur
	VS_IL_Init(&pSSAOBlurVS, IL.posNormalTexture, &pPosNormalTexIL,
		IL.nPosNormalTexture, L"Shaders\\Vertex\\SSAOBlurVS.cso");
	PS_Init(&pSSAOBlurPS, L"Shaders\\Pixel\\SSAOBlurPS.cso");


	//debug quad
	VS_IL_Init(&pDebugQuadVS, IL.posNormalTexture, &pPosNormalTexIL,
		IL.nPosNormalTexture, L"Shaders\\Vertex\\DebugScreenQuadVS.cso");
	PS_Init(&pDebugQuadPS, L"Shaders\\Pixel\\DebugScreenQuadPS.cso");


	//Default Shaders
	//normal map and posNormalTexcTangent layout
	VS_IL_Init(&pNormalMapVS, IL.posNormalTexcTangent, &pPosNormalTexcTangentIL, IL.nPosNormalTexcTangent, L"Shaders\\Vertex\\NormalMapVS.cso");
	PS_Init(&pNormalMapPS, L"Shaders\\Pixel\\NormalMapPS.cso");
	//shadow map
	VS_Init(&pShadowMapVS, L"Shaders\\Vertex\\ShadowMapVS.cso");
	PS_Init(&pShadowMapPS, L"Shaders\\Pixel\\ShadowMapPS.cso");
	//default light
	VS_IL_Init(&pDefaultLightVS, IL.posNormalTexcTangent, &pPosNormalTexcTangentIL, IL.nPosNormalTexcTangent, L"Shaders\\Vertex\\DefaultLightVS.cso");
	PS_Init(&pDefaultLightPS, L"Shaders\\Pixel\\DefaultLightPS.cso");

	//Displacement waves
	VS_IL_Init(&pDisplacementWavesVS, IL.posNormalTexcTangent, &pPosNormalTexcTangentIL,
		IL.nPosNormalTexcTangent, L"Shaders\\Vertex\\TessellationWavesVS.cso");
	PS_Init(&pDisplacementWavesPS, L"Shaders\\Pixel\\TessellationWavesPS.cso");
	HS_Init(&pDisplacementWavesHS, L"Shaders\\Hull\\TessellationWavesHS.cso");
	DS_Init(&pDisplacementWavesDS, L"Shaders\\Domain\\TessellationWavesDS.cso");

	//Terrain
	VS_IL_Init(&pTerrainVS, IL.terrainHeightMap, &pTerrainIL, IL.nTerrainHeightMap, L"Shaders\\Vertex\\Terrain_VS.cso");
	HS_Init(&pTerrainHS, L"Shaders\\Hull\\Terrain_HS.cso");
	DS_Init(&pTerrainDS, L"Shaders\\Domain\\Terrain_DS.cso");
	PS_Init(&pTerrainPS, L"Shaders\\Pixel\\Terrain_PS.cso");

	//////////////////////////////////////////////////////////////////////////
	//Particle Stream out
	VS_IL_Init(&pParticleStreamOutVS, IL.particleStreamOutIL, &pParticleStreamOutIL, IL.nParticleStreamOut, L"Shaders\\Vertex\\StreamOutVS.cso");
	//Particles fire
	GS_SO_Init(&pSOFireGS, L"Shaders\\Geometry\\FireSOGS.cso");
	VS_IL_Init(&pParticleFireVS, IL.particleDrawIL, &pParticleDrawIL, IL.nParticleDraw, L"Shaders\\Vertex\\FireVS.cso");
	GS_Init(&pParticleFireGS, L"Shaders\\Geometry\\FireGS.cso");
	PS_Init(&pParticleFirePS, L"Shaders\\Pixel\\ParticlePS.cso");
	//particles rain
	GS_SO_Init(&pParticleRainGSSO, L"Shaders\\Geometry\\RainSOGS.cso");
	GS_Init(&pParticleRainGS, L"Shaders\\Geometry\\RainGS.cso");
	VS_IL_Init(&pParticleRainVS, IL.particleDrawIL, &pParticleDrawIL, IL.nParticleDraw, L"Shaders\\Vertex\\RainVS.cso");
	PS_Init(&pParticleRainPS, L"Shaders\\Pixel\\RainPS.cso");

	//particles explosion
	GS_SO_Init(&pParticleExplosionSOGS, L"Shaders\\Geometry\\ExplosionSOGS.cso");
	GS_Init(&pParticleExplosionGS, L"Shaders\\Geometry\\ExplosionGS.cso");
	VS_IL_Init(&pParticleExplosionVS, IL.particleDrawIL, &pParticleDrawIL, IL.nParticleDraw, L"Shaders\\Vertex\\ExplosionVS.cso");

	//fountain
	GS_SO_Init(&pParticleFountainSOGS, L"Shaders\\Geometry\\FountainSOGS.cso");
	GS_Init(&pParticleFountainGS, L"Shaders\\Geometry\\FountainGS.cso");
	VS_IL_Init(&pParticleFountainVS, IL.particleDrawIL, &pParticleDrawIL, IL.nParticleDraw, L"Shaders\\Vertex\\FountainVS.cso");
	//////////////////////////////////////////////////////////////////////////
	//compute shader waves 
	VS_IL_Init(&pComputeWavesVS, IL.posNormalTexture, &pPosNormalTexIL, IL.nPosNormalTexture, L"Shaders\\Vertex\\ComputeWavesVS.cso");
	PS_Init(&pComputeWavesPS, L"Shaders\\Pixel\\ComputeWavesPS.cso");
	CS_Init(&pComputeWavesDisturbCS, L"Shaders\\Compute\\DisturbWavesCS.cso");
	CS_Init(&pComputeWavesUpdateCS, L"Shaders\\Compute\\UpdateWavesCS.cso");

}

void Graphics::BindVSandIA(ShaderPicker shader)
{
	switch (shader)
	{
	case ShaderPicker::Sky_VS_PS:
	{
		pgfx_pDeviceContext->IASetInputLayout(pSkyIL);
		pgfx_pDeviceContext->VSSetShader(pSkyVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::DisplacementWaves_VS_HS_DS_PS:
	{
		pgfx_pDeviceContext->IASetInputLayout(pPosNormalTexcTangentIL);
		pgfx_pDeviceContext->VSSetShader(pDisplacementWavesVS, nullptr, 0u);
		break;

	}
	case ShaderPicker::TerrainHeightMap:
	{
		pgfx_pDeviceContext->IASetInputLayout(pTerrainIL);
		pgfx_pDeviceContext->VSSetShader(pTerrainVS, nullptr, 0u);
		break;

	}
	case ShaderPicker::Particles_FireStreamOut_VS_GS:
	case ShaderPicker::Particles_RainStreamOut_VS_GS:
	case ShaderPicker::Particles_ExplosionStreamOut_VS_GS:
	case ShaderPicker::Particle_FountainStreamOut_VS_GS:
	{
		pgfx_pDeviceContext->IASetInputLayout(pParticleStreamOutIL);//this one
		pgfx_pDeviceContext->VSSetShader(pParticleStreamOutVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Particles_FireDraw_VS_GS_PS:
	{
		pgfx_pDeviceContext->IASetInputLayout(pParticleDrawIL);
		pgfx_pDeviceContext->VSSetShader(pParticleFireVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Particles_RainDraw_VS_GS_PS:
	{
		pgfx_pDeviceContext->IASetInputLayout(pParticleDrawIL);
		pgfx_pDeviceContext->VSSetShader(pParticleRainVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Particle_ExplosionDraw_VS_GS_PS:
	{
		pgfx_pDeviceContext->IASetInputLayout(pParticleDrawIL);
		pgfx_pDeviceContext->VSSetShader(pParticleExplosionVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Particle_FountainDraw_VS_GS_PS:
	{
		pgfx_pDeviceContext->IASetInputLayout(pParticleDrawIL);
		pgfx_pDeviceContext->VSSetShader(pParticleFountainVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::ComputeSSAO_VS_PS:
	{
		pgfx_pDeviceContext->IASetInputLayout(pPosNormalTexIL);
		pgfx_pDeviceContext->VSSetShader(pSSAOFullScreenQuadVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::DrawDebugTexQuad_VS_PS:
	{
		pgfx_pDeviceContext->IASetInputLayout(pPosNormalTexIL);
		pgfx_pDeviceContext->VSSetShader(pDebugQuadVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::SSAOBlur_VS_PS:
	{
		pgfx_pDeviceContext->IASetInputLayout(pPosNormalTexIL);
		pgfx_pDeviceContext->VSSetShader(pSSAOBlurVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::NormalMap_VS_PS:
	{
		pgfx_pDeviceContext->IASetInputLayout(pPosNormalTexcTangentIL);
		pgfx_pDeviceContext->VSSetShader(pNormalMapVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::ShadowMap_VS_PS:
	{
		pgfx_pDeviceContext->IASetInputLayout(pPosNormalTexcTangentIL);
		pgfx_pDeviceContext->VSSetShader(pShadowMapVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::DefaultLight_VS_PS:
	{
		pgfx_pDeviceContext->IASetInputLayout(pPosNormalTexcTangentIL);
		pgfx_pDeviceContext->VSSetShader(pDefaultLightVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::ComputeWaves_VS_PS_CS:
	{
		pgfx_pDeviceContext->IASetInputLayout(pPosNormalTexIL);
		pgfx_pDeviceContext->VSSetShader(pComputeWavesVS, nullptr, 0u);
		break;
	}
	default:
		break;
	}
}

void Graphics::BindPS(ShaderPicker shader)
{
	switch (shader)
	{
	case ShaderPicker::Sky_VS_PS:
	{
		pgfx_pDeviceContext->PSSetShader(pSkyPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::DisplacementWaves_VS_HS_DS_PS:
	{
		pgfx_pDeviceContext->PSSetShader(pDisplacementWavesPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::TerrainHeightMap:
	{
		pgfx_pDeviceContext->PSSetShader(pTerrainPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Particles_FireDraw_VS_GS_PS:
	case ShaderPicker::Particle_ExplosionDraw_VS_GS_PS:
	{
		pgfx_pDeviceContext->PSSetShader(pParticleFirePS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Particles_RainDraw_VS_GS_PS:
	case ShaderPicker::Particle_FountainDraw_VS_GS_PS:
	{
		pgfx_pDeviceContext->PSSetShader(pParticleRainPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::ComputeSSAO_VS_PS:
	{
		pgfx_pDeviceContext->PSSetShader(pSSAOFullScreenQuadPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::DrawDebugTexQuad_VS_PS:
	{
		pgfx_pDeviceContext->PSSetShader(pDebugQuadPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::SSAOBlur_VS_PS:
	{
		pgfx_pDeviceContext->PSSetShader(pSSAOBlurPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::NormalMap_VS_PS:
	{
		pgfx_pDeviceContext->PSSetShader(pNormalMapPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::ShadowMap_VS_PS:
	{
		pgfx_pDeviceContext->PSSetShader(pShadowMapPS, nullptr, 0u);
		break;

	case ShaderPicker::DefaultLight_VS_PS:
	{
		pgfx_pDeviceContext->PSSetShader(pDefaultLightPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::ComputeWaves_VS_PS_CS:
	{
		pgfx_pDeviceContext->PSSetShader(pComputeWavesPS, nullptr, 0u);
		break;
	}
	default:
		break;
	}
	}
}
void Graphics::BindGS(ShaderPicker shader)
{
	switch (shader)
	{
	case ShaderPicker::Particles_FireStreamOut_VS_GS:
	{
		pgfx_pDeviceContext->GSSetShader(pSOFireGS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Particles_FireDraw_VS_GS_PS:
	{
		pgfx_pDeviceContext->GSSetShader(pParticleFireGS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Particles_RainStreamOut_VS_GS:
	{
		pgfx_pDeviceContext->GSSetShader(pParticleRainGSSO, nullptr, 0u);
		break;
	}
	case ShaderPicker::Particles_RainDraw_VS_GS_PS:
	{
		pgfx_pDeviceContext->GSSetShader(pParticleRainGS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Particle_ExplosionDraw_VS_GS_PS:
	{
		pgfx_pDeviceContext->GSSetShader(pParticleExplosionGS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Particles_ExplosionStreamOut_VS_GS:
	{
		pgfx_pDeviceContext->GSSetShader(pParticleExplosionSOGS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Particle_FountainStreamOut_VS_GS:
	{
		pgfx_pDeviceContext->GSSetShader(pParticleFountainSOGS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Particle_FountainDraw_VS_GS_PS:
	{
		pgfx_pDeviceContext->GSSetShader(pParticleFountainGS, nullptr, 0u);
		break;
	}
	default:
		break;
	}
}

void Graphics::BindCS(ShaderPicker shader)
{
	switch (shader)
	{
	case ShaderPicker::DisturbWaves_CS:
	{
		pgfx_pDeviceContext->CSSetShader(pComputeWavesDisturbCS, nullptr, 0u);
		break;
	}
	case ShaderPicker::UpdateWaves_CS:
	{
		pgfx_pDeviceContext->CSSetShader(pComputeWavesUpdateCS, nullptr, 0u);
		break;
	}
	}
}

void Graphics::BindHS(ShaderPicker shader)
{
	switch (shader)
	{
	case ShaderPicker::DisplacementWaves_VS_HS_DS_PS:
	{
		pgfx_pDeviceContext->HSSetShader(pDisplacementWavesHS, 0u, 0u);
		break;
	}
	case ShaderPicker::TerrainHeightMap:
	{
		pgfx_pDeviceContext->HSSetShader(pTerrainHS, 0u, 0u);
		break;
	}
	}
}

void Graphics::BindDS(ShaderPicker shader)
{
	switch (shader)
	{
	case ShaderPicker::DisplacementWaves_VS_HS_DS_PS:
	{
		pgfx_pDeviceContext->DSSetShader(pDisplacementWavesDS, 0u, 0u);
		break;
	}
	case ShaderPicker::TerrainHeightMap:
	{
		pgfx_pDeviceContext->DSSetShader(pTerrainDS, 0u, 0u);
		break;
	}
	}
}

void Graphics::UnbindCS()
{
	pgfx_pDeviceContext->CSSetShader(0u, nullptr, 0u);
}

void Graphics::UnbindGS()
{
	pgfx_pDeviceContext->GSSetShader(0u, nullptr, 0u);
}

void Graphics::UnbindPS()
{
	pgfx_pDeviceContext->CSSetShader(0u, nullptr, 0u);
}

void Graphics::UnbindVS()
{
	pgfx_pDeviceContext->VSSetShader(0u, nullptr, 0u);
}

void Graphics::UnbindHS()
{
	pgfx_pDeviceContext->HSSetShader(0u, nullptr, 0u);
}

void Graphics::UnbindDS()
{
	pgfx_pDeviceContext->DSSetShader(0u, nullptr, 0u);
}

void Graphics::UnbindAll()
{
	UnbindCS();
	UnbindDS();
	UnbindGS();
	UnbindHS();
	UnbindPS();
	UnbindVS();
}

void Graphics::VS_IL_Init(ID3D11VertexShader** pVShader,
	const D3D11_INPUT_ELEMENT_DESC* inputLayout,
	ID3D11InputLayout** pIL,
	UINT nElements, const std::wstring& path)
{
#ifdef MY_DEBUG
	CheckFileExistence(path);
#endif // MY_DEBUG
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), &pBlob));
	DX::ThrowIfFailed(pgfx_pDevice->CreateVertexShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		pVShader));
#ifdef MY_DEBUG
	if (path != std::wstring())
	{
		SetDebugName(*pVShader, path.c_str());
	}
#endif
	InitializeInputLayout(inputLayout, pIL, nElements, pBlob, L"VertexShader_");

	//for usage in other shaders;
	pBlob->Release();

}

void Graphics::VS_Init(ID3D11VertexShader** pVShader, const std::wstring& path)
{
#ifdef MY_DEBUG
	CheckFileExistence(path);
#endif // MY_DEBUG
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), &pBlob));
	DX::ThrowIfFailed(pgfx_pDevice->CreateVertexShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		pVShader));
#ifdef MY_DEBUG
	if (path != std::wstring())
	{
		SetDebugName(*pVShader, path.c_str());
	}
#endif
	//for usage in other shaders;
	pBlob->Release();
}

void Graphics::PS_Init(ID3D11PixelShader** pPSShader, const std::wstring& path)
{
#ifdef MY_DEBUG
	CheckFileExistence(path);
#endif // MY_DEBUG
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), &pBlob));
	DX::ThrowIfFailed(pgfx_pDevice->CreatePixelShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		pPSShader));
#ifdef MY_DEBUG
	if (debugDevice != nullptr)
	{
		SetDebugName(*pPSShader, path.c_str());
	}
#endif
	//for usage in other shaders;
	pBlob->Release();

}

void Graphics::InitializeInputLayout(const D3D11_INPUT_ELEMENT_DESC* inputLayout,
	ID3D11InputLayout** pIL,
	UINT nElements,
	ID3DBlob* pBlob, const std::wstring& name)
{
	DX::ThrowIfFailed(pgfx_pDevice->CreateInputLayout(
		inputLayout,
		nElements,
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		pIL));
#ifdef MY_DEBUG
	if (name != std::wstring())
	{
		SetDebugName(*pIL, name.c_str());
	}
#endif
}

void Graphics::GS_Init(ID3D11GeometryShader** pGSShader, const std::wstring& path)
{
#ifdef MY_DEBUG
	CheckFileExistence(path);
#endif // MY_DEBUG
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), &pBlob));
	DX::ThrowIfFailed(pgfx_pDevice->CreateGeometryShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		pGSShader));
#ifdef MY_DEBUG
	if (debugDevice != nullptr)
	{
		SetDebugName(*pGSShader, path.c_str());
	}
#endif
	//for usage in other shaders;
	pBlob->Release();
}

void Graphics::GS_SO_Init(ID3D11GeometryShader** pGSShader, const std::wstring& path)
{
#ifdef MY_DEBUG
	CheckFileExistence(path);
#endif // MY_DEBUG
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), &pBlob));
	DX::ThrowIfFailed(pgfx_pDevice->CreateGeometryShaderWithStreamOutput(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		SO.fire,
		SO.fireSize,
		NULL, 0, D3D11_SO_NO_RASTERIZED_STREAM, nullptr,
		pGSShader));
#ifdef MY_DEBUG
	if (debugDevice != nullptr)
	{
		SetDebugName(*pGSShader, path.c_str());
	}
#endif
	//for usage in other shaders;
	pBlob->Release();
}

void Graphics::CS_Init(ID3D11ComputeShader** pCShader, const std::wstring& path)
{
#ifdef MY_DEBUG
	CheckFileExistence(path);
#endif // MY_DEBUG
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), &pBlob));
	DX::ThrowIfFailed(pgfx_pDevice->CreateComputeShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		pCShader));
#ifdef MY_DEBUG
	if (debugDevice != nullptr)
	{
		SetDebugName(*pCShader, path.c_str());
	}
#endif
	//for usage in other shaders;
	pBlob->Release();
}

void Graphics::HS_Init(ID3D11HullShader** pHShader, const std::wstring& path)
{
#ifdef MY_DEBUG
	CheckFileExistence(path);
#endif // MY_DEBUG
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), &pBlob));
	DX::ThrowIfFailed(pgfx_pDevice->CreateHullShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		pHShader));
#ifdef MY_DEBUG
	if (debugDevice != nullptr)
	{
		SetDebugName(*pHShader, path.c_str());
	}
#endif
	//for usage in other shaders;
	pBlob->Release();

}

void Graphics::DS_Init(ID3D11DomainShader** pDshader, const std::wstring& path)
{
#ifdef MY_DEBUG
	CheckFileExistence(path);
#endif // MY_DEBUG
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), &pBlob));
	DX::ThrowIfFailed(pgfx_pDevice->CreateDomainShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		pDshader));
#ifdef MY_DEBUG
	if (debugDevice != nullptr)
	{
		SetDebugName(*pDshader, path.c_str());
	}
#endif
	//for usage in other shaders;
	pBlob->Release();

}


void Graphics::InitializeRenderStates()
{
	//
	//ShadowMap Bias
	//
	D3D11_RASTERIZER_DESC shadowMapBiasDesc;
	ZeroMemory(&shadowMapBiasDesc, sizeof(D3D11_RASTERIZER_DESC));
	shadowMapBiasDesc.FillMode = D3D11_FILL_SOLID;
	shadowMapBiasDesc.CullMode = D3D11_CULL_BACK;
	shadowMapBiasDesc.FrontCounterClockwise = false;
	shadowMapBiasDesc.DepthClipEnable = true;

	shadowMapBiasDesc.DepthBias = 100000;
	shadowMapBiasDesc.DepthBiasClamp = 0.0f;
	shadowMapBiasDesc.SlopeScaledDepthBias = 1.0f;
	pgfx_pDevice->CreateRasterizerState(&shadowMapBiasDesc, &ShadowMapBiasRS);

	//
	//SolidFIllRS
	//
	D3D11_RASTERIZER_DESC solidFillDesc;
	ZeroMemory(&solidFillDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidFillDesc.FillMode = D3D11_FILL_SOLID;
	solidFillDesc.CullMode = D3D11_CULL_NONE;
	solidFillDesc.FrontCounterClockwise = false;
	solidFillDesc.DepthClipEnable = true;
	pgfx_pDevice->CreateRasterizerState(&solidFillDesc, &SolidFillRS);


	//
	// WireframeRS
	//
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	pgfx_pDevice->CreateRasterizerState(&wireframeDesc, &WireframeRS);

	//
	// NoCullRS
	//
	D3D11_RASTERIZER_DESC noCullDesc;
	ZeroMemory(&noCullDesc, sizeof(D3D11_RASTERIZER_DESC));
	noCullDesc.FillMode = D3D11_FILL_SOLID;
	noCullDesc.CullMode = D3D11_CULL_NONE;
	noCullDesc.FrontCounterClockwise = false;
	noCullDesc.DepthClipEnable = true;

	pgfx_pDevice->CreateRasterizerState(&noCullDesc, &NoCullRS);

	//
	// CullClockwiseRS
	//

	// Note: Define such that we still cull backfaces by making front faces CCW.
	// If we did not cull backfaces, then we have to worry about the BackFace
	// property in the D3D11_DEPTH_STENCIL_DESC.
	D3D11_RASTERIZER_DESC cullClockwiseDesc;
	ZeroMemory(&cullClockwiseDesc, sizeof(D3D11_RASTERIZER_DESC));
	cullClockwiseDesc.FillMode = D3D11_FILL_SOLID;
	cullClockwiseDesc.CullMode = D3D11_CULL_BACK;
	cullClockwiseDesc.FrontCounterClockwise = true;
	cullClockwiseDesc.DepthClipEnable = true;

	pgfx_pDevice->CreateRasterizerState(&cullClockwiseDesc, &CullClockwiseRS);

	////Cull counterCloskwise
	D3D11_RASTERIZER_DESC cullCounterClockwiseDesc;
	ZeroMemory(&cullCounterClockwiseDesc, sizeof(D3D11_RASTERIZER_DESC));
	cullCounterClockwiseDesc.FillMode = D3D11_FILL_SOLID;
	cullCounterClockwiseDesc.CullMode = D3D11_CULL_FRONT;
	cullCounterClockwiseDesc.FrontCounterClockwise = true;
	cullCounterClockwiseDesc.DepthClipEnable = true;

	pgfx_pDevice->CreateRasterizerState(&cullCounterClockwiseDesc, &CullCounterClockwiseRS);


	//
	// AlphaToCoverageBS
	//

	D3D11_BLEND_DESC alphaToCoverageDesc = { 0 };
	alphaToCoverageDesc.AlphaToCoverageEnable = true;
	alphaToCoverageDesc.IndependentBlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].BlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	pgfx_pDevice->CreateBlendState(&alphaToCoverageDesc, &AlphaToCoverageBS);

	//
	// TransparentBS
	//

	D3D11_BLEND_DESC transparentDesc = { 0 };
	transparentDesc.AlphaToCoverageEnable = false;
	transparentDesc.IndependentBlendEnable = false;

	transparentDesc.RenderTarget[0].BlendEnable = true;
	transparentDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	transparentDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	transparentDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	transparentDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	transparentDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	pgfx_pDevice->CreateBlendState(&transparentDesc, &TransparentBS);

	//
	//noBlendBS
	//
	D3D11_BLEND_DESC noBlendBSDesc = { 0 };
	noBlendBSDesc.AlphaToCoverageEnable = false;
	noBlendBSDesc.IndependentBlendEnable = false;

	noBlendBSDesc.RenderTarget[0].BlendEnable = false;
	noBlendBSDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	noBlendBSDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	noBlendBSDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	noBlendBSDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	noBlendBSDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	noBlendBSDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	noBlendBSDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	pgfx_pDevice->CreateBlendState(&noBlendBSDesc, &noBlendBS);


	//
	//Additive
	//
	D3D11_BLEND_DESC additiveDesc = { 0 };
	additiveDesc.AlphaToCoverageEnable = false;
	additiveDesc.IndependentBlendEnable = false;

	additiveDesc.RenderTarget[0].BlendEnable = true;
	additiveDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	additiveDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	additiveDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	additiveDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	additiveDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	additiveDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	additiveDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	pgfx_pDevice->CreateBlendState(&additiveDesc, &additiveBlend);

	//colorful one
	D3D11_BLEND_DESC test = { 0 };
	test.AlphaToCoverageEnable = false;
	test.IndependentBlendEnable = false;

	test.RenderTarget[0].BlendEnable = true;
	test.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR;
	test.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	test.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	test.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	test.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	test.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	test.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	pgfx_pDevice->CreateBlendState(&test, &srsColor);

	//
	// NoRenderTargetWritesBS
	//

	D3D11_BLEND_DESC noRenderTargetWritesDesc = { 0 };
	noRenderTargetWritesDesc.AlphaToCoverageEnable = false;
	noRenderTargetWritesDesc.IndependentBlendEnable = false;

	noRenderTargetWritesDesc.RenderTarget[0].BlendEnable = false;
	noRenderTargetWritesDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	noRenderTargetWritesDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	noRenderTargetWritesDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	noRenderTargetWritesDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	noRenderTargetWritesDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	noRenderTargetWritesDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	noRenderTargetWritesDesc.RenderTarget[0].RenderTargetWriteMask = 0;

	pgfx_pDevice->CreateBlendState(&noRenderTargetWritesDesc, &NoRenderTargetWritesBS);

	//
	// MarkMirrorDSS
	//

	D3D11_DEPTH_STENCIL_DESC mirrorDesc;
	mirrorDesc.DepthEnable = true;
	mirrorDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	mirrorDesc.DepthFunc = D3D11_COMPARISON_LESS;
	mirrorDesc.StencilEnable = true;
	mirrorDesc.StencilReadMask = 0xff;
	mirrorDesc.StencilWriteMask = 0xff;

	mirrorDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	mirrorDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// We are not rendering backfacing polygons, so these settings do not matter.
	mirrorDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	mirrorDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	pgfx_pDevice->CreateDepthStencilState(&mirrorDesc, &MarkMirrorDSS);

	//
	// DrawReflectionDSS
	//

	D3D11_DEPTH_STENCIL_DESC drawReflectionDesc;
	drawReflectionDesc.DepthEnable = true;
	drawReflectionDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	drawReflectionDesc.DepthFunc = D3D11_COMPARISON_LESS;
	drawReflectionDesc.StencilEnable = true;
	drawReflectionDesc.StencilReadMask = 0xff;
	drawReflectionDesc.StencilWriteMask = 0xff;

	drawReflectionDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	// We are not rendering backfacing polygons, so these settings do not matter.
	drawReflectionDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	pgfx_pDevice->CreateDepthStencilState(&drawReflectionDesc, &DrawReflectionDSS);

	//
	// NoDoubleBlendDSS
	//

	D3D11_DEPTH_STENCIL_DESC noDoubleBlendDesc;
	noDoubleBlendDesc.DepthEnable = true;
	noDoubleBlendDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	noDoubleBlendDesc.DepthFunc = D3D11_COMPARISON_LESS;
	noDoubleBlendDesc.StencilEnable = true;
	noDoubleBlendDesc.StencilReadMask = 0xff;
	noDoubleBlendDesc.StencilWriteMask = 0xff;

	noDoubleBlendDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	noDoubleBlendDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	noDoubleBlendDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	noDoubleBlendDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	// We are not rendering backfacing polygons, so these settings do not matter.
	noDoubleBlendDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	noDoubleBlendDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	noDoubleBlendDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	noDoubleBlendDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	pgfx_pDevice->CreateDepthStencilState(&noDoubleBlendDesc, &NoDoubleBlendDSS);

	//
	//DepthComplexityCounter
	//
	D3D11_DEPTH_STENCIL_DESC depthComplCountDesc;
	depthComplCountDesc.DepthEnable = true;
	depthComplCountDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthComplCountDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	depthComplCountDesc.StencilEnable = true;
	depthComplCountDesc.StencilReadMask = 0xff;
	depthComplCountDesc.StencilWriteMask = 0xff;

	depthComplCountDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INCR;
	depthComplCountDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthComplCountDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	depthComplCountDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// We are not rendering backfacing polygons, so these settings do not matter.
	depthComplCountDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthComplCountDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthComplCountDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	depthComplCountDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;


	pgfx_pDevice->CreateDepthStencilState(&depthComplCountDesc, &DepthComplexityCountDSS);

	//
	//DepthComplexityRead
	//
	D3D11_DEPTH_STENCIL_DESC depthComplReadDesc;
	depthComplReadDesc.DepthEnable = true;
	depthComplReadDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthComplReadDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	depthComplReadDesc.StencilEnable = true;
	depthComplReadDesc.StencilReadMask = 0xff;
	depthComplReadDesc.StencilWriteMask = 0xff;

	depthComplReadDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthComplReadDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthComplReadDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthComplReadDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	// We are not rendering backfacing polygons, so these settings do not matter.
	depthComplReadDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthComplReadDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthComplReadDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	depthComplReadDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;


	pgfx_pDevice->CreateDepthStencilState(&depthComplReadDesc, &DepthComplexityReadDSS);

	//
	//Disable depth writes, keep depth test
	//
	D3D11_DEPTH_STENCIL_DESC disableDepthWritesDesc;
	disableDepthWritesDesc.DepthEnable = true;
	disableDepthWritesDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	disableDepthWritesDesc.DepthFunc = D3D11_COMPARISON_LESS;
	disableDepthWritesDesc.StencilEnable = false;
	disableDepthWritesDesc.StencilReadMask = 0xff;
	disableDepthWritesDesc.StencilWriteMask = 0xff;

	disableDepthWritesDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	disableDepthWritesDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	disableDepthWritesDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	disableDepthWritesDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	// We are not rendering backfacing polygons, so these settings do not matter.
	disableDepthWritesDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	disableDepthWritesDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	disableDepthWritesDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	disableDepthWritesDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;


	pgfx_pDevice->CreateDepthStencilState(&disableDepthWritesDesc, &disableDepthWrites);
	//
	//lessEqualDss
	//
	D3D11_DEPTH_STENCIL_DESC lessEqualDSSDesc;
	lessEqualDSSDesc.DepthEnable = true;
	lessEqualDSSDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	lessEqualDSSDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	lessEqualDSSDesc.StencilEnable = false;


	pgfx_pDevice->CreateDepthStencilState(&lessEqualDSSDesc, &LessEqualDSS);

	//
	//EqualDSS
	//
	D3D11_DEPTH_STENCIL_DESC equalDSSDesc;
	equalDSSDesc.DepthEnable = true;
	equalDSSDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	equalDSSDesc.DepthFunc = D3D11_COMPARISON_EQUAL;
	equalDSSDesc.StencilEnable = false;
	pgfx_pDevice->CreateDepthStencilState(&equalDSSDesc, &EqualDSS);

}

void Graphics::DestroyRenderStates()
{
	ReleaseID3D(WireframeRS);
	ReleaseID3D(NoCullRS);
	ReleaseID3D(CullClockwiseRS);
	ReleaseID3D(CullCounterClockwiseRS);
	ReleaseID3D(SolidFillRS);
	ReleaseID3D(ShadowMapBiasRS);

	ReleaseID3D(AlphaToCoverageBS);
	ReleaseID3D(TransparentBS);
	ReleaseID3D(NoRenderTargetWritesBS);
	ReleaseID3D(additiveBlend);
	ReleaseID3D(noBlendBS);

	ReleaseID3D(MarkMirrorDSS);
	ReleaseID3D(DrawReflectionDSS);
	ReleaseID3D(NoDoubleBlendDSS);
	ReleaseID3D(DepthComplexityCountDSS);
	ReleaseID3D(DepthComplexityReadDSS);
	ReleaseID3D(LessEqualDSS);
	ReleaseID3D(disableDepthWrites);
	ReleaseID3D(EqualDSS);
}

