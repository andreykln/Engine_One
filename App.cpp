
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "App.h"
App::App()
	: wnd("Output Window", resolution_width, resolution_height)
{
	rStates.InitializeAll(wnd.GetGraphics());
	pShaders = new Shaders(wnd.GetGraphics());
	const int smapSize = 2048;
	pShadowMap = new ShadowMapGen(wnd.GetGraphics(), smapSize, smapSize);
	pSSAO = new SSAO(wnd.GetGraphics(), resolution_width, resolution_height);
	viewProjectionMatrix = GetViewProjectionCamera();
// 	wnd.GetGraphics().SetMatrices(viewProjectionMatrix, camera.GetViewMatrix());
	pDC = wnd.GetGraphics().pgfx_pDeviceContext.Get();

	///
	/// init
	////
	wnd.GetGraphics().CreateCBuffers();
	wnd.GetGraphics().CreateSRVs();
	wnd.GetGraphics().CreateAndBindSamplers();
	wnd.GetGraphics().CreateRuntimeCBuffers(pSSAO->GetAndBuildConstantBufferData());
	////////
	///////
	/////////

// 	CreateBilateralHillsBlur();
// 	CreateBox();

	CreateShadowMapDemo();
// 	CreateHillsWithGPUWaves();

// 	CreateDepthComplexityStencil();
// 	CreateGaussBlur();
// 	CreateBezierPatchTess();
// 	CreateTerrain();
// 	InstancingCreate();

}

void App::DoFrame()
{

// 	const float c = abs((sin(timer.TotalTime())));
	timer.Tick();

	DrawShadowMapDemo();
// 	DrawHillsWithGPUWaves();


// 	DrawBox();
// 	DrawDepthComplexityStencil();
// 	DrawGaussBlur();
// 	DrawBilateralHillsBlur();
// 	DrawBezierPatchTess();
// 	DrawTerrain();
// 	DrawInstancingDraw();

	CalculateFrameStats();

// 	DebugTextToTitle();
	wnd.GetGraphics().EndFrame();
// 	wnd.GetGraphics().ClearBuffer(0.69f, 0.77f, 0.87f);
	wnd.GetGraphics().ClearBuffer(0.0392f, 0.0392f, 0.17254f);

}

int App::Go()
{
	timer.Reset();
	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}
		DoFrame();
	}
	return 0;
}


void App::DebugTextToTitle()
{
	std::ostringstream oss;
	oss << "X: " << wnd.mouse.GetPosX();
	wnd.SetTitle(oss.str().c_str());
}



void App::CalculateFrameStats()
{
	static int frameCount = 0;
	static float timeElapsed = 0.0f;
	frameCount++;
	if ((timer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = static_cast<float>(frameCount);
		float ms_per_frame = 1000.0f / fps;
			std::ostringstream oss;
			oss << "FPS:" << fps
				<< "; Frame Time: " << ms_per_frame << " ms., TotalTime: " << static_cast<UINT>(timer.TotalTime());
			wnd.SetTitle(oss.str().c_str());
		frameCount = 0;
		timeElapsed += 1.0f;
	}
}





void App::CreateHillsWithGPUWaves()
{
	pWaveSurfaceGPU = new WaveSurfaceGPU(wnd.GetGraphics());
}

void App::DrawHillsWithGPUWaves()
{
// 	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::WireframeRS);
	viewProjectionMatrix = GetViewProjectionCamera();
	wnd.GetGraphics().pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(RenderStates::TransparentBS, blendFactorsZero, 0xffffffff);
	pShaders->BindVSandIA(ShaderPicker::GPUWaves_VS);
	pShaders->BindPS(ShaderPicker::LightAndTexture_VS_PS);
	pWaveSurfaceGPU->UpdateVSMatrices(wnd.GetGraphics(), DirectX::XMMatrixTranslation(0.0f, -5.0f, 0.0f), viewProjectionMatrix,
		timer.DeltaTime());
	pWaveSurfaceGPU->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
	pWaveSurfaceGPU->BindAndDrawIndexed(wnd.GetGraphics());
	pWaveSurfaceGPU->ClearVertexShaderResource(wnd.GetGraphics());
	// every quarter second, generate a random wave
	static float t_base = 0.0f;
	if ((timer.TotalTime() - t_base) >= 0.1f)
	{
		pShaders->BindCS(ShaderPicker::DisturbWaves_CS);
		t_base += 0.1f;
		pWaveSurfaceGPU->Disturb(wnd.GetGraphics());
		pShaders->UnbindCS();
	}
	pShaders->BindCS(ShaderPicker::UpdateWaves_CS);
	pWaveSurfaceGPU->UpdateSolution(wnd.GetGraphics(), timer.DeltaTime());
	pShaders->UnbindAll();
}

void App::CreateBilateralHillsBlur()
{
	CreateHillsWithGPUWaves();
	pGaussianBlur = new GaussianBlur(wnd.GetGraphics());

}

void App::DrawBilateralHillsBlur()
{
	const float color[] = { 0.69f, 0.77f, 0.87f, 1.0f };
	wnd.GetGraphics().pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	viewProjectionMatrix = GetViewProjectionCamera();
	//offscreen render target
	ID3D11RenderTargetView* renderTargets[1] = { pGaussianBlur->GetRTV() };

	wnd.GetGraphics().pgfx_pDeviceContext->OMSetRenderTargets(1u, renderTargets, wnd.GetGraphics().pgfx_DepthStencilView.Get());
	wnd.GetGraphics().pgfx_pDeviceContext->ClearRenderTargetView(pGaussianBlur->GetRTV(), color);
	wnd.GetGraphics().pgfx_pDeviceContext->ClearDepthStencilView(wnd.GetGraphics().pgfx_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	wnd.GetGraphics().SetViewport();
	DrawHillsWithGPUWaves();
	//set default render target
	renderTargets[0] = wnd.GetGraphics().pgfx_RenderTargetView.Get();
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetRenderTargets(1u, renderTargets, wnd.GetGraphics().pgfx_DepthStencilView.Get());

	pShaders->BindCS(ShaderPicker::HorizontalBilateralBlur_CS);
	pGaussianBlur->PerformHorizontalBlur(wnd.GetGraphics());
	pShaders->UnbindCS();
	pShaders->BindCS(ShaderPicker::VerticalBilateralBlur_CS);
	pGaussianBlur->PerformVerticalBlur(wnd.GetGraphics());
	pShaders->UnbindCS();
	//reset before drawing quad
	//wnd.GetGraphics().pgfx_pDeviceContext->OMSetRenderTargets(1u, renderTargets, wnd.GetGraphics().pgfx_DepthStencilView.Get());
// 	wnd.GetGraphics().ClearBuffer(0.69f, 0.77f, 0.87f);
	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::BlurTexture_PS);
	//quad
	pGaussianBlur->UpdateVSMatrices(wnd.GetGraphics(), DirectX::XMMatrixIdentity(), viewProjectionMatrix);
	pGaussianBlur->BindAndDrawIndexed(wnd.GetGraphics());

	//clear resource so it can be used for RTV in a new frame
	ID3D11ShaderResourceView* nullSRV = nullptr;
	wnd.GetGraphics().pgfx_pDeviceContext->PSSetShaderResources(0u, 1u, &nullSRV);

}

void App::CreateGaussBlur()
{
// 	CreateHillsWithWavesAllLight();
	CreateHillsWithGPUWaves();

	pGaussianBlur = new GaussianBlur(wnd.GetGraphics());
}

void App::DrawGaussBlur()
{
	const float color[] = { 0.69f, 0.77f, 0.87f, 1.0f };
	wnd.GetGraphics().pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	viewProjectionMatrix = GetViewProjectionCamera();
	//offscreen render target
	ID3D11RenderTargetView* renderTargets[1] = { pGaussianBlur->GetRTV() };

	wnd.GetGraphics().pgfx_pDeviceContext->OMSetRenderTargets(1u, renderTargets, wnd.GetGraphics().pgfx_DepthStencilView.Get());
	wnd.GetGraphics().pgfx_pDeviceContext->ClearRenderTargetView(pGaussianBlur->GetRTV(), color);
	wnd.GetGraphics().pgfx_pDeviceContext->ClearDepthStencilView(wnd.GetGraphics().pgfx_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	wnd.GetGraphics().SetViewport();
// 	DrawHillsWithWavesAllLight();
	DrawHillsWithGPUWaves();

	//set default render target
	renderTargets[0] = wnd.GetGraphics().pgfx_RenderTargetView.Get();
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetRenderTargets(1u, renderTargets, wnd.GetGraphics().pgfx_DepthStencilView.Get());

	pShaders->BindCS(ShaderPicker::HorizontalBlur_CS);
	pGaussianBlur->PerformHorizontalBlur(wnd.GetGraphics());
	pShaders->UnbindCS();
	pShaders->BindCS(ShaderPicker::VerticalBlur_CS);
	pGaussianBlur->PerformVerticalBlur(wnd.GetGraphics());
	pShaders->UnbindCS();
	//reset before drawing quad
	//wnd.GetGraphics().pgfx_pDeviceContext->OMSetRenderTargets(1u, renderTargets, wnd.GetGraphics().pgfx_DepthStencilView.Get());
// 	wnd.GetGraphics().ClearBuffer(0.69f, 0.77f, 0.87f);
	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::BlurTexture_PS);
	//quad
	pGaussianBlur->UpdateVSMatrices(wnd.GetGraphics(), DirectX::XMMatrixIdentity(), viewProjectionMatrix);
	pGaussianBlur->BindAndDrawIndexed(wnd.GetGraphics());

	//clear resource so it can be used for RTV in a new frame
	ID3D11ShaderResourceView* nullSRV = nullptr;
	wnd.GetGraphics().pgfx_pDeviceContext->PSSetShaderResources(0u, 1u, &nullSRV);
}

void App::CreateBox()
{
	pBox = new Box(wnd.GetGraphics(), 3.0f, 3.0f, 3.0f, DemoSwitch::DefaultBox);
}

void App::DrawBox()
{
	viewProjectionMatrix = GetViewProjectionCamera();

	pBox->DrawTestBox(wnd.GetGraphics(), DirectX::XMMatrixIdentity(), viewProjectionMatrix, camera.GetCameraPosition(), pShaders);
	pBox->BindAndDrawIndexed(wnd.GetGraphics());
}


void App::DrawDepthComplexityStencil()
{
	wnd.GetGraphics().pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	viewProjectionMatrix = GetViewProjectionCamera();
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(RenderStates::DepthComplexityCountDSS, 0);
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::CullCounterClockwiseRS);


	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::LightAndTexture_VS_PS);

// 	pHills->UpdateVSMatrices(wnd.GetGraphics(), pHills->GetHillsOffset(), viewProjectionMatrix, camera.GetCameraPosition());
// 	pHills->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
	pHills->BindAndDrawIndexed(wnd.GetGraphics());

// disable transparency for overdraw counting
// 	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(RenderStates::TransparentBS, blendFactorsZero, 0xffffffff);
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::NoCullRS);
// 
// 	pBox->UpdateVSMatrices(wnd.GetGraphics(), pBox->GetBoxForHillsOffset(), viewProjectionMatrix);
// 	pBox->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
	pBox->BindAndDrawIndexed(wnd.GetGraphics());



	pShaders->BindVSandIA(ShaderPicker::DepthComplexityVS_PS);
	pShaders->BindPS(ShaderPicker::DepthComplexityVS_PS);

	pDepthArr[0]->UpdateVSMatrices(wnd.GetGraphics(), DirectX::XMMatrixTranslation(0.0f, 0.0f, 3.0f), viewProjectionMatrix);
	pDepthArr[1]->UpdateVSMatrices(wnd.GetGraphics(), DirectX::XMMatrixTranslation(0.0f, 0.0f, 3.0f), viewProjectionMatrix);
	pDepthArr[2]->UpdateVSMatrices(wnd.GetGraphics(), DirectX::XMMatrixTranslation(0.0f, 0.0f, 3.0f), viewProjectionMatrix);

	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(RenderStates::DepthComplexityReadDSS, 3);
	pDepthArr[0]->UpdateDepthComplexityColor(wnd.GetGraphics(), DirectX::XMFLOAT3{ 0.5f, 0.0f, 0.0f });
	pDepthArr[0]->BindAndDrawIndexed(wnd.GetGraphics());

	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(RenderStates::DepthComplexityReadDSS, 2);
	pDepthArr[1]->UpdateDepthComplexityColor(wnd.GetGraphics(), DirectX::XMFLOAT3{ 1.0f, 0.54f, 0.117f });
	pDepthArr[1]->BindAndDrawIndexed(wnd.GetGraphics());

	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(RenderStates::DepthComplexityReadDSS, 1);
	pDepthArr[2]->UpdateDepthComplexityColor(wnd.GetGraphics(), DirectX::XMFLOAT3{ 0.0f, 0.5f, 0.0f });
	pDepthArr[2]->BindAndDrawIndexed(wnd.GetGraphics());


}

void App::CreateDepthComplexityStencil()
{
	pDepthArr[0] = new DepthComplexity(wnd.GetGraphics());
	pDepthArr[1] = new DepthComplexity(wnd.GetGraphics());
	pDepthArr[2] = new DepthComplexity(wnd.GetGraphics());
	pHills = new Hills(wnd.GetGraphics(), 160.0f, 160.0f, 50u, 50u);
// 	pBox = new Box(wnd.GetGraphics(), 5.0f, 5.0f, 5.0f, DemoSwitch::HillsDemo);

}


DirectX::XMMATRIX App::GetViewProjectionCamera()
{
	bool reset = false;


	return 	viewProjectionMatrix = camera.GetViewProjectionWithMovement(
		wnd.mouse.GetPosX(),
		wnd.mouse.GetPosY(),
		(float)resolution_width / (float)resolution_height,
		0.1f,
		reset,
		timer.DeltaTime(),
		wnd);
}




void App::SetDefaultRTVAndViewPort()
{
	ID3D11RenderTargetView* renderTargets[1]{ wnd.GetGraphics().pgfx_RenderTargetView.Get() };
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetRenderTargets(1u, &renderTargets[0], wnd.GetGraphics().pgfx_DepthStencilView.Get());
	wnd.GetGraphics().pgfx_pDeviceContext->ClearRenderTargetView(renderTargets[0], colors);
	wnd.GetGraphics().pgfx_pDeviceContext->ClearDepthStencilView(
		wnd.GetGraphics().pgfx_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	wnd.GetGraphics().SetViewport();
}

void App::DrawSceneToShadowMap()
{

	pShadowMap->BuildShadowTransform(pShadowMap->GetNewLightDirection());

	DirectX::XMMATRIX VP = pShadowMap->GetLighViewProjection();
	wnd.GetGraphics().ConstBufferShadowMapBind();
	stride = sizeof(vbPosNormalTexTangent);
	offset = 0; 
	//skull
	wnd.GetGraphics().ShadowMap(pSkull->skullWorld, VP);
	pDC->IASetVertexBuffers(0u, 1u, pSkull->GetVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pSkull->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	pDC->DrawIndexed(pSkull->GetIndexCount(), 0u, 0u);

	//cylinders
	pDC->IASetVertexBuffers(0u, 1u, pCylinder->GetVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pCylinder->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	const UINT mCylIndCount = pCylinder->GetIndexCount();
	for (int i = 0; i < 10; i++)
	{
		wnd.GetGraphics().ShadowMap(pCylinder->m_CylWorld[i], VP);
		pDC->DrawIndexed(mCylIndCount, 0u, 0u);
	}
	//plate
	pDC->IASetVertexBuffers(0u, 1u, pPlate->GetVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pPlate->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	wnd.GetGraphics().ShadowMap(DirectX::XMMatrixIdentity(), VP);;
	pDC->DrawIndexed(pPlate->GetIndexCount(), 0u, 0u);

	//cube
	pDC->IASetVertexBuffers(0u, 1u, pBox->GetVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pBox->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	wnd.GetGraphics().ShadowMap(shapes.Get_m_BoxWorld(), VP);;
	pDC->DrawIndexed(pBox->GetIndexCount(), 0u, 0u);
	//spheres
	pDC->IASetVertexBuffers(0u, 1u, pGeoSphere->GetVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pGeoSphere->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	const UINT mSphereIndCount = pGeoSphere->GetIndexCount();
	for (int i = 0; i < 10; i++)
	{
		wnd.GetGraphics().ShadowMap(pGeoSphere->m_SphereWorld[i], VP);
		pDC->DrawIndexed(mSphereIndCount, 0u, 0u);
	}

}

void App::CreateShadowMapDemo()
{
	pSky = new Sky(wnd.GetGraphics());
	pBox = new Box(wnd.GetGraphics(), 1.0f, 1.0f, 2.0f, DemoSwitch::ShadowMap);
 	pSkull = new Skull(wnd.GetGraphics(), L"models\\skull.txt");
	pPlate = new Hills(wnd.GetGraphics(), 25.0f, 25.0f, 45, 45);
	pCylinder = new Cylinder(wnd.GetGraphics(), 0.5f, 0.3f, 3.0f, 20, 20);
	pGeoSphere = new GeoSphere(wnd.GetGraphics(), 0.5f, 2u, false, DemoSwitch::ShadowMap);


	wnd.GetGraphics().BindCubeMap(pSky->skyBoxName);

}

void App::DrawShadowMapDemo()
{
	pDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	//update every frame
	viewProjectionMatrix = GetViewProjectionCamera();
	wnd.GetGraphics().SetMatrices(viewProjectionMatrix, camera.GetViewMatrix(), camera.GetProjecion());

	//////////////////////////////////////////////////////////////////////////
	pShaders->BindVSandIA(ShadowMap_VS_PS);
	pShaders->BindPS(ShadowMap_VS_PS);
	pShadowMap->BindDSVandSetNullRenderTarget(wnd.GetGraphics());
	pShadowMap->UpdateScene(timer.DeltaTime());
	pDC->RSSetState(RenderStates::ShadowMapBiasRS);
	DrawSceneToShadowMap();
	pDC->RSSetState(0u);

	//normal map
	pSSAO->SetNormalDepthRenderTarget(wnd.GetGraphics(), wnd.GetGraphics().pgfx_DepthStencilView.Get());
	DrawNormalMap(viewProjectionMatrix);


	pShaders->BindVSandIA(ComputeSSAO_VS_PS);
	pShaders->BindPS(ComputeSSAO_VS_PS);
	stride = sizeof(vbPosNormalTex);
	pDC->IASetVertexBuffers(0u, 1u, pSSAO->GetQuadVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pSSAO->GetQuadIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);

	wnd.GetGraphics().ComputeSSAO(pSSAO->GetAmbientMapRTV0(), pSSAO->GetSSAOViewport(), 
		pSSAO->GetRandomVectorSRV(), pSSAO->GetNormalMapSRV());
	pDC->DrawIndexed(pSSAO->GetQuadIndexCount(), 0u, 0u);

	pShaders->BindVSandIA(SSAOBlur_VS_PS);
	pShaders->BindPS(SSAOBlur_VS_PS);
	wnd.GetGraphics().BlurSSAOMap(4, pSSAO->GetAmbientMapRTV0(), pSSAO->GetAmbientMapRTV1(), pSSAO->GetAmbientMapSRV0(),
		pSSAO->GetAmbientMapSRV1(), pSSAO->GetSSAOViewport());
	pShaders->UnbindVS();
	pShaders->UnbindPS();
	wnd.GetGraphics().ReleaseNormalMapResource();
	SetDefaultRTVAndViewPort();

	//////////////////////////////////////////////////////////////////////////
	
	

	
	auto newLightDirection = pShadowMap->GetNewLightDirection();

	//Skull
// 	pShaders->BindVSandIA(ShaderPicker::ShadowMapDrawSkull_VS_PS);
// 	pShaders->BindPS(ShaderPicker::ShadowMapDrawSkull_VS_PS);
// 	pSkull->UpdateShadowMapDrawBuffers(wnd.GetGraphics(), camera.GetCameraPosition(), pShadowMap->GetShadowTransform(),
// 		shapes.GetCameraOffset() * DirectX::XMMatrixTranslation(0.0f, 6.0f, 0.0f)
// 		* DirectX::XMMatrixScaling(0.3f, 0.3f, 0.3f), viewProjectionMatrix,
// 		pShadowMap->DepthMapSRV(), newLightDirection);
// 	pSkull->BindAndDrawIndexed(wnd.GetGraphics());

// 	pShaders->BindVSandIA(ShaderPicker::ShadowMapInstancedDraw_VS);
// 	pShaders->BindPS(ShaderPicker::DefaultInstanced_PS);
	//columns
// 	pInstancedCylinder->UpdateDrawInstancedBuffers(wnd.GetGraphics(), camera.GetCameraPosition(),
// 		pShadowMap->GetShadowTransform(), viewProjectionMatrix,
// 		pShadowMap->DepthMapSRV(), pShadowMap->GetNewLightDirection());
// 	pInstancedCylinder->BindAndDrawInstancedIndexed(wnd.GetGraphics(), 10, 0, 0, 0);

// 	wnd.GetGraphics().pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
// 	pShaders->BindHS(ShaderPicker::DisplacementMapping_VS_DS_HS);
// 	pShaders->BindDS(ShaderPicker::DisplacementMapping_VS_DS_HS);

// 	pShaders->UnbindDS();
// 	pShaders->UnbindHS();
// 	wnd.GetGraphics().pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//spheres
// 	pInstancedGeoSphere->UpdateShadowMapDrawInstancedBuffers(wnd.GetGraphics(), camera.GetCameraPosition(),
// 		pShadowMap->GetShadowTransform(), viewProjectionMatrix,
// 		pShadowMap->DepthMapSRV(), pShadowMap->GetNewLightDirection());
// 	pInstancedGeoSphere->BindAndDrawInstancedIndexed(wnd.GetGraphics(), 10, 0, 0, 0);


	//plane
	wnd.GetGraphics().ConstBufferVSMatricesBind();
	pShaders->BindVSandIA(DefaultLight_VS_PS);
	pShaders->BindPS(DefaultLight_VS_PS);
	stride = sizeof(vbPosNormalTexTangent);
	pDC->IASetVertexBuffers(0u, 1u, pPlate->GetVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pPlate->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	bool usessao;
	if (GetAsyncKeyState(GetAsyncKeyState('5') & 0x8000))
		usessao = false;
	else
		usessao = true;
	wnd.GetGraphics().VSDefaultMatricesUpdate(ID, pPlate->plateScaling, pShadowMap->GetShadowTransform(),
		ID, camera.GetCameraPosition());

	wnd.GetGraphics().SetDefaultLightData();
	wnd.GetGraphics().DefaultLightUpdate(pPlate->plateMaterial, camera.GetCameraPosition(),
		false, newLightDirection, usessao, pPlate->diffuseMap, pPlate->normalMap);

	pDC->PSSetShaderResources(2u, 1u, pShadowMap->DepthMapSRV());
	pDC->DrawIndexed(pPlate->GetIndexCount(), 0u, 0u);
// 	pShaders->BindVSandIA(ShaderPicker::ShadowMap_VS_PS);
// 	pHills->UpdateShadowMapDrawBuffers(wnd.GetGraphics(), camera.GetCameraPosition(),
// 		pShadowMap->GetShadowTransform(), shapes.Get_m_GridWorld() * shapes.GetCameraOffset(),
// 		viewProjectionMatrix, pShadowMap->DepthMapSRV(), pShadowMap->GetNewLightDirection());
// 	pShadowMap->SetShadowSampler(wnd.GetGraphics());
// 	pHills->BindAndDrawIndexed(wnd.GetGraphics());
// 	//box
// 	pDisplacementMappingBox->UpdateShadowMapDrawBuffers(wnd.GetGraphics(), camera.GetCameraPosition(),
// 		pShadowMap->GetShadowTransform(), shapes.Get_m_BoxWorld() * shapes.GetCameraOffset(), viewProjectionMatrix,
// 		pShadowMap->DepthMapSRV(), pShadowMap->GetNewLightDirection());
// 	pDisplacementMappingBox->BindAndDrawIndexed(wnd.GetGraphics());

	//////////////////////////////////////////////////////////////////////////
	////	DEBUG
// 	pSSAO->DrawDebugScreenQuad(wnd.GetGraphics(), pShaders);
	pShaders->BindVSandIA(DrawDebugTexQuad_VS_PS);
	pShaders->BindPS(DrawDebugTexQuad_VS_PS);
	stride = sizeof(vbPosNormalTex);
	pDC->IASetVertexBuffers(0u, 1u, pSSAO->GetQuadVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pSSAO->GetQuadIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
// 	ID3D11ShaderResourceView* pNMSRV = pSSAO->GetNormalMapSRV();
	ID3D11ShaderResourceView* pNMSRV = pSSAO->GetAmbientMapSRV0();

	pDC->PSSetShaderResources(5u, 1u, &pNMSRV);

	pDC->DrawIndexed(pSSAO->GetQuadIndexCount(), 0u, 0u);
	ID3D11ShaderResourceView* pNullSRV = nullptr;
	//release for the SSAO pass
	pDC->PSSetShaderResources(5u, 1u, &pNullSRV);



	pDC->RSSetState(RenderStates::NoCullRS);
	pDC->OMSetDepthStencilState(RenderStates::LessEqualDSS, 0u);

	pShaders->BindVSandIA(ShaderPicker::Sky_VS_PS);
	pShaders->BindPS(ShaderPicker::Sky_VS_PS);
	wnd.GetGraphics().ConstBufferVSMatricesBind();
	wnd.GetGraphics().VSDefaultMatricesUpdate(ID, ID, ID, ID, camera.GetCameraPosition());
	stride = sizeof(DirectX::XMFLOAT3);
	pDC->IASetVertexBuffers(0u, 1u, pSky->GetVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pSky->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	pDC->DrawIndexed(pSky->GetIndexCount(), 0u, 0u);



	pDC->RSSetState(0u);
	pDC->OMSetDepthStencilState(0u, 0u);
	//release shadow map SRV
	ID3D11ShaderResourceView* pNULLSRV = nullptr;
	pDC->PSSetShaderResources(2u, 1u, &pNULLSRV);
	//////////////////////////////////////////////////////////////////////////

	
}

void App::DrawNormalMap(DirectX::XMMATRIX viewProjectionMatrix)
{
	wnd.GetGraphics().ConstBufferNormalMapBind();
	pShaders->BindVSandIA(NormalMap_VS_PS);
	pShaders->BindPS(NormalMap_VS_PS);

	const UINT stride = sizeof(vbPosNormalTexTangent);
	const UINT offset = 0;
	//skull
	wnd.GetGraphics().NormalMap(pSkull->skullWorld);
	pDC->IASetVertexBuffers(0u, 1u, pSkull->GetVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pSkull->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	pDC->DrawIndexed(pSkull->GetIndexCount(), 0u, 0u);

	//cylinders
	pDC->IASetVertexBuffers(0u, 1u, pCylinder->GetVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pCylinder->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	const UINT mCylIndCount = pCylinder->GetIndexCount();
	for (int i = 0; i < 10; i++)
	{
		wnd.GetGraphics().NormalMap(pCylinder->m_CylWorld[i]);
		pDC->DrawIndexed(mCylIndCount, 0u, 0u);
	}
	//plate
	pDC->IASetVertexBuffers(0u, 1u, pPlate->GetVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pPlate->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	wnd.GetGraphics().NormalMap(DirectX::XMMatrixIdentity());;
	pDC->DrawIndexed(pPlate->GetIndexCount(), 0u, 0u);

	//cube
	pDC->IASetVertexBuffers(0u, 1u, pBox->GetVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pBox->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	wnd.GetGraphics().NormalMap(shapes.Get_m_BoxWorld());;
	pDC->DrawIndexed(pBox->GetIndexCount(), 0u, 0u);
	//spheres
	pDC->IASetVertexBuffers(0u, 1u, pGeoSphere->GetVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pGeoSphere->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	const UINT mSphereIndCount = pGeoSphere->GetIndexCount();
	for (int i = 0; i < 10; i++)
	{
		wnd.GetGraphics().NormalMap(pGeoSphere->m_SphereWorld[i]);
		pDC->DrawIndexed(mSphereIndCount, 0u, 0u);
	}
}

void App::CreateBezierPatchTess()
{
	pQuadTess = new QuadTessellation(wnd.GetGraphics());
}

void App::DrawBezierPatchTess()
{
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::WireframeRS);

	viewProjectionMatrix = GetViewProjectionCamera();
	pQuadTess->UpdateTessellationShaderBuffers(wnd.GetGraphics(), viewProjectionMatrix, DirectX::XMMatrixTranslation(0.0f, -25.0f, 0.0f), camera.GetCameraPosition());

	pShaders->BindVSandIA(QuadTessellation_VS);
	pShaders->BindHS(QuadTessellation_HS);
	pShaders->BindDS(QuadTessellation_DS);
	pShaders->BindPS(QuadTessellation_PS);
	pQuadTess->BindAndDraw(wnd.GetGraphics(), 16u, 0u);
}

void App::InstancingCreate()
{
	pInstancedSkulls = new InstancedSkull(wnd.GetGraphics());
}

void App::DrawInstancingDraw()
{
	viewProjectionMatrix = GetViewProjectionCamera();
	pShaders->BindVSandIA(ShaderPicker::InstancedSkull_VS);
	pShaders->BindPS(ShaderPicker::InstancedSkull_PS);
	pInstancedSkulls->UpdateVSMatrices(wnd.GetGraphics(), viewProjectionMatrix, camera.GetViewMatrix(), camera.GetProjecion());
	pInstancedSkulls->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
	pInstancedSkulls->BindAndDrawInstancedIndexed(wnd.GetGraphics(), pInstancedSkulls->GetAmountOfVisible(), 0u, 0u, 0u);
}




void App::CreateTerrain()
{
  	//pTerrain = new Terrain(wnd.GetGraphics());

 	pParticle = new ParticleSystem(wnd.GetGraphics(), 500);
 	//pParticleRain = new ParticleSystem(wnd.GetGraphics(), 6000);
 	pParticleExplosion = new ParticleSystem(wnd.GetGraphics(), 2500);

	pParticleFountain = new ParticleSystem(wnd.GetGraphics(), 2000);
}


void App::DrawTerrain()
{
	viewProjectionMatrix = GetViewProjectionCamera();

	// TERRAIN
	/*pShaders->UnbindAll();
	pShaders->BindVSandIA(ShaderPicker::TerrainHeightMap_VS_PS_DS_HS_PS);
	pShaders->BindHS(ShaderPicker::TerrainHeightMap_VS_PS_DS_HS_PS);
	pShaders->BindDS(ShaderPicker::TerrainHeightMap_VS_PS_DS_HS_PS);
	pShaders->BindPS(ShaderPicker::TerrainHeightMap_VS_PS_DS_HS_PS);

	//bind camera to terrain surface
// 	DirectX::XMFLOAT3 camPos = camera.GetCameraPosition();
// 	float y = pTerrain->GetHeight(camPos.x, camPos.z, camPos.y);
// 	camera.SetCameraYPosition(y + 2.0f);
	wnd.GetGraphics().pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	if (GetAsyncKeyState('6') & 0x8000)
	{
		wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::WireframeRS);
	}

	pTerrain->SetSRVAndCBuffers(wnd.GetGraphics(), camera.GetCameraPosition(), viewProjectionMatrix);
	pTerrain->BindAndDrawIndexed(wnd.GetGraphics(), pTerrain->GetNumQuadFaces() * 4, 0u, 0u);*/


	// PARTICLES
	pShaders->UnbindAll();
	wnd.GetGraphics().pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	//RAIN
	/*DirectX::XMFLOAT3 rainPosition = camera.GetCameraPosition();
	rainPosition.z += 10.0f;
	rainPosition.y += 5.0f;
	pParticleRain->DrawParticle(wnd.GetGraphics(), pShaders, viewProjectionMatrix, camera.GetCameraPosition(),
		rainPosition, timer.DeltaTime(), timer.TotalTime(), ParticlePick::Rain);

 	pShaders->UnbindAll();*/

	//FOUNTAIN
	pParticleFountain->DrawParticle(wnd.GetGraphics(), pShaders, viewProjectionMatrix, camera.GetCameraPosition(),
		DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), timer.DeltaTime(), timer.TotalTime(), ParticlePick::Fountain);

	pShaders->UnbindAll();



	//EXPLOSION
	pParticleExplosion->DrawParticle(wnd.GetGraphics(), pShaders, viewProjectionMatrix, camera.GetCameraPosition(),
		DirectX::XMFLOAT3(0.0f, 3.0f, 1.0f), timer.DeltaTime(), timer.TotalTime(), ParticlePick::Explosion);

	pShaders->UnbindAll();

	//FIRE
	//supposed to be drawn last so it will blend
	pParticle->DrawParticle(wnd.GetGraphics(), pShaders, viewProjectionMatrix, camera.GetCameraPosition(),
		DirectX::XMFLOAT3(50.0f, 0.0f, 1.0f), timer.DeltaTime(), timer.TotalTime(), ParticlePick::Fire);




	//reset
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(0u, blendFactorsZero, 0xffffffff);
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(0u, 0u);

}

App::~App()
{
}
