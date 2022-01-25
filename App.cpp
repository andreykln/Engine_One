// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "App.h"
App::App()
	: wnd("Output Window", resolution_width, resolution_height)
{
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

	cbComputeSSAOconstData ssaoData = pSSAO->GetAndBuildConstantBufferData();
	wnd.GetGraphics().CreateRuntimeCBuffers(ssaoData, cbNames.ssaoConstData, "ssao constant data");

	

	////////

// 	CreateBilateralHillsBlur();
// 	CreateBox();

// 	CreateShadowMapDemo();
// 	CreateHillsWithGPUWaves();
	CreateTerrain();

// 	CreateDepthComplexityStencil();
// 	CreateGaussBlur();
// 	CreateBezierPatchTess();
// 	InstancingCreate();

}

void App::DoFrame()
{
	viewProjectionMatrix = GetViewProjectionCamera();

// 	const float c = abs((sin(timer.TotalTime())));
	timer.Tick();
	wnd.GetGraphics().SetCommonShaderConstants(viewProjectionMatrix, camera.GetViewMatrix(),
		camera.GetProjecion(), camera.GetCameraPosition(), timer.DeltaTime(), timer.TotalTime());

// 	DrawShadowMapDemo();
// 	DrawHillsWithGPUWaves();
	DrawTerrain();


// 	DrawBox();
// 	DrawDepthComplexityStencil();
// 	DrawGaussBlur();
// 	DrawBilateralHillsBlur();
// 	DrawBezierPatchTess();
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
	cbGPUWavesVSConstData computeWavesVSData = pWaveSurfaceGPU->GetAndBuildConstantBufferData();
	wnd.GetGraphics().CreateRuntimeCBuffers(computeWavesVSData, cbNames.computeWavesVSData, "compute Waves initial VS Data");
	wnd.GetGraphics().SetWavesCSResources(pWaveSurfaceGPU->pPreviousSolutionSRV,
		pWaveSurfaceGPU->pCurrentSolutionSRV,
		pWaveSurfaceGPU->pNextSolutionSRV,
		pWaveSurfaceGPU->pPreviousSolutionUAV,
		pWaveSurfaceGPU->pCurrentSolutionUAV,
		pWaveSurfaceGPU->pNextSolutionUAV);
	pSky = new Sky(wnd.GetGraphics());
	wnd.GetGraphics().BindCubeMap(pSky->skyBoxName);

}

void App::DrawHillsWithGPUWaves()
{
// 	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(wnd.GetGraphics().WireframeRS);
// 	viewProjectionMatrix = GetViewProjectionCamera();
	wnd.GetGraphics().SetDefaultLightData();

	wnd.GetGraphics().SetComputeWavesSamplers();
	wnd.GetGraphics().pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	wnd.GetGraphics().SetComputeWavesResources();
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(wnd.GetGraphics().TransparentBS, blendFactorsZero, 0xffffffff);

	wnd.GetGraphics().BindVSandIA(ShaderPicker::ComputeWaves_VS_PS_CS);
	wnd.GetGraphics().BindPS(ShaderPicker::ComputeWaves_VS_PS_CS);
	wnd.GetGraphics().UpdateComputeWaves(pWaveSurfaceGPU->wavesWorld);
	wnd.GetGraphics().DefaultLightUpdate(pWaveSurfaceGPU->wavesMat, false, false, pWaveSurfaceGPU->diffuseMap, pWaveSurfaceGPU->normalMap);
	stride = sizeof(vbPosNormalTex);
	offset = 0;

	pDC->IASetVertexBuffers(0u, 1u, pWaveSurfaceGPU->GetVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pWaveSurfaceGPU->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	pDC->DrawIndexed(pWaveSurfaceGPU->GetIndexCount(), 0u, 0u);
	wnd.GetGraphics().ComputeWavesClearVS();
	static float t_base = 0.0f;
	if ((timer.TotalTime() - t_base) >= 0.1f)
	{
		wnd.GetGraphics().BindCS(ShaderPicker::DisturbWaves_CS);
		t_base += 0.1f;
		wnd.GetGraphics().DisturbComputeWaves(pWaveSurfaceGPU->numColumns, pWaveSurfaceGPU->numRows, pWaveSurfaceGPU->waveConstant);
		wnd.GetGraphics().UnbindCS();
	}
	wnd.GetGraphics().BindCS(ShaderPicker::UpdateWaves_CS);
	wnd.GetGraphics().UpdateSolutionComputeWaves(pWaveSurfaceGPU->numColumns, pWaveSurfaceGPU->numRows);
	wnd.GetGraphics().UnbindAll();


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

// 	wnd.GetGraphics().BindCS(ShaderPicker::HorizontalBilateralBlur_CS);
// 	pGaussianBlur->PerformHorizontalBlur(wnd.GetGraphics());
// 	wnd.GetGraphics().UnbindCS();
// 	wnd.GetGraphics().BindCS(ShaderPicker::VerticalBilateralBlur_CS);
// 	pGaussianBlur->PerformVerticalBlur(wnd.GetGraphics());
// 	wnd.GetGraphics().UnbindCS();
// 	//reset before drawing quad
// 	//wnd.GetGraphics().pgfx_pDeviceContext->OMSetRenderTargets(1u, renderTargets, wnd.GetGraphics().pgfx_DepthStencilView.Get());
// // 	wnd.GetGraphics().ClearBuffer(0.69f, 0.77f, 0.87f);
// 	wnd.GetGraphics().BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
// 	wnd.GetGraphics().BindPS(ShaderPicker::BlurTexture_PS);
	//quad
	pGaussianBlur->UpdateVSMatrices(wnd.GetGraphics(), DirectX::XMMatrixIdentity(), viewProjectionMatrix);
// 	pGaussianBlur->BindAndDrawIndexed(wnd.GetGraphics());

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

// 	wnd.GetGraphics().BindCS(ShaderPicker::HorizontalBlur_CS);
// 	pGaussianBlur->PerformHorizontalBlur(wnd.GetGraphics());
// 	wnd.GetGraphics().UnbindCS();
// 	wnd.GetGraphics().BindCS(ShaderPicker::VerticalBlur_CS);
// 	pGaussianBlur->PerformVerticalBlur(wnd.GetGraphics());
// 	wnd.GetGraphics().UnbindCS();
// 	//reset before drawing quad
// 	//wnd.GetGraphics().pgfx_pDeviceContext->OMSetRenderTargets(1u, renderTargets, wnd.GetGraphics().pgfx_DepthStencilView.Get());
// // 	wnd.GetGraphics().ClearBuffer(0.69f, 0.77f, 0.87f);
// 	wnd.GetGraphics().BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
// 	wnd.GetGraphics().BindPS(ShaderPicker::BlurTexture_PS);
// 	//quad
	pGaussianBlur->UpdateVSMatrices(wnd.GetGraphics(), DirectX::XMMatrixIdentity(), viewProjectionMatrix);
// 	pGaussianBlur->BindAndDrawIndexed(wnd.GetGraphics());

	//clear resource so it can be used for RTV in a new frame
	ID3D11ShaderResourceView* nullSRV = nullptr;
	wnd.GetGraphics().pgfx_pDeviceContext->PSSetShaderResources(0u, 1u, &nullSRV);
}

void App::CreateBox()
{
	pBox = new Box(wnd.GetGraphics(), 3.0f, 3.0f, 3.0f);
}


void App::DrawDepthComplexityStencil()
{
	wnd.GetGraphics().pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	viewProjectionMatrix = GetViewProjectionCamera();
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(wnd.GetGraphics().DepthComplexityCountDSS, 0);
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(wnd.GetGraphics().CullCounterClockwiseRS);


// 	wnd.GetGraphics().BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
// 	wnd.GetGraphics().BindPS(ShaderPicker::LightAndTexture_VS_PS);

// 	pHills->UpdateVSMatrices(wnd.GetGraphics(), pHills->GetHillsOffset(), viewProjectionMatrix, camera.GetCameraPosition());
// 	pHills->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
// 	pHills->BindAndDrawIndexed(wnd.GetGraphics());

// disable transparency for overdraw counting
// 	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(wnd.GetGraphics().TransparentBS, blendFactorsZero, 0xffffffff);
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(wnd.GetGraphics().NoCullRS);
// 
// 	pBox->UpdateVSMatrices(wnd.GetGraphics(), pBox->GetBoxForHillsOffset(), viewProjectionMatrix);
// 	pBox->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
// 	pBox->BindAndDrawIndexed(wnd.GetGraphics());



// 	wnd.GetGraphics().BindVSandIA(ShaderPicker::DepthComplexityVS_PS);
// 	wnd.GetGraphics().BindPS(ShaderPicker::DepthComplexityVS_PS);

	pDepthArr[0]->UpdateVSMatrices(wnd.GetGraphics(), DirectX::XMMatrixTranslation(0.0f, 0.0f, 3.0f), viewProjectionMatrix);
	pDepthArr[1]->UpdateVSMatrices(wnd.GetGraphics(), DirectX::XMMatrixTranslation(0.0f, 0.0f, 3.0f), viewProjectionMatrix);
	pDepthArr[2]->UpdateVSMatrices(wnd.GetGraphics(), DirectX::XMMatrixTranslation(0.0f, 0.0f, 3.0f), viewProjectionMatrix);

	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(wnd.GetGraphics().DepthComplexityReadDSS, 3);
	pDepthArr[0]->UpdateDepthComplexityColor(wnd.GetGraphics(), DirectX::XMFLOAT3{ 0.5f, 0.0f, 0.0f });
// 	pDepthArr[0]->BindAndDrawIndexed(wnd.GetGraphics());

	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(wnd.GetGraphics().DepthComplexityReadDSS, 2);
	pDepthArr[1]->UpdateDepthComplexityColor(wnd.GetGraphics(), DirectX::XMFLOAT3{ 1.0f, 0.54f, 0.117f });
// 	pDepthArr[1]->BindAndDrawIndexed(wnd.GetGraphics());

	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(wnd.GetGraphics().DepthComplexityReadDSS, 1);
	pDepthArr[2]->UpdateDepthComplexityColor(wnd.GetGraphics(), DirectX::XMFLOAT3{ 0.0f, 0.5f, 0.0f });
// 	pDepthArr[2]->BindAndDrawIndexed(wnd.GetGraphics());


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
	pBox = new Box(wnd.GetGraphics(), 1.0f, 1.0f, 2.0f);
 	pSkull = new Skull(wnd.GetGraphics(), L"models\\skull.txt");
	pPlate = new Hills(wnd.GetGraphics(), 25.0f, 25.0f, 45, 45);
	pCylinder = new Cylinder(wnd.GetGraphics(), 0.5f, 0.3f, 3.0f, 20, 20);
	pGeoSphere = new GeoSphere(wnd.GetGraphics(), 0.5f, 2u);
	pDispWaves = new DisplacementWaves(wnd.GetGraphics());
	wnd.GetGraphics().BindCubeMap(pSky->skyBoxName);
}

void App::DrawShadowMapDemo()
{
	pDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	//update every frame
	viewProjectionMatrix = GetViewProjectionCamera();
// 	wnd.GetGraphics().SetCommonShaderConstants(viewProjectionMatrix, camera.GetViewMatrix(),
// 		camera.GetProjecion(), camera.GetCameraPosition(), timer.TotalTime());

	//shadow map
	wnd.GetGraphics().BindVSandIA(ShadowMap_VS_PS);
	wnd.GetGraphics().BindPS(ShadowMap_VS_PS);
	pShadowMap->BindDSVandSetNullRenderTarget(wnd.GetGraphics());
	pShadowMap->UpdateScene(timer.DeltaTime());
	pDC->RSSetState(wnd.GetGraphics().ShadowMapBiasRS);
	DrawSceneToShadowMap();
	pDC->RSSetState(0u);

	//create normal-depth map
	pSSAO->SetNormalDepthRenderTarget(wnd.GetGraphics(), wnd.GetGraphics().pgfx_DepthStencilView.Get());
	DrawNormalMap(viewProjectionMatrix);

	//SSAO
	wnd.GetGraphics().BindVSandIA(ComputeSSAO_VS_PS);
	wnd.GetGraphics().BindPS(ComputeSSAO_VS_PS);
	stride = sizeof(vbPosNormalTex);
	pDC->IASetVertexBuffers(0u, 1u, pSSAO->GetQuadVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pSSAO->GetQuadIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	wnd.GetGraphics().ComputeSSAO(pSSAO->GetAmbientMapRTV0(), pSSAO->GetSSAOViewport(), 
		pSSAO->GetRandomVectorSRV(), pSSAO->GetNormalMapSRV());
	pDC->DrawIndexed(pSSAO->GetQuadIndexCount(), 0u, 0u);

	// blur
	wnd.GetGraphics().BindVSandIA(SSAOBlur_VS_PS);
	wnd.GetGraphics().BindPS(SSAOBlur_VS_PS);
	wnd.GetGraphics().BlurSSAOMap(4, pSSAO->GetAmbientMapRTV0(), pSSAO->GetAmbientMapRTV1(), pSSAO->GetAmbientMapSRV0(),
		pSSAO->GetAmbientMapSRV1(), pSSAO->GetSSAOViewport());
	wnd.GetGraphics().UnbindVS();
	wnd.GetGraphics().UnbindPS();
	wnd.GetGraphics().ReleaseNormalMapResource();
	SetDefaultRTVAndViewPort();

	//////////////////////////////////////////////////////////////////////////
	// init main drawing buffers
	stride = sizeof(vbPosNormalTexTangent);
	wnd.GetGraphics().ConstBufferVSMatricesBind();
	pDC->PSSetShaderResources(2u, 1u, pShadowMap->DepthMapSRV());
	bool usessao = true;
	if (GetAsyncKeyState('5') & 0x8000)
		usessao = false;
	else
		usessao = true;
	wnd.GetGraphics().SetDefaultLightData();
	wnd.GetGraphics().UpdateLightDirection(pShadowMap->GetNewLightDirection());
	wnd.GetGraphics().SetShadowTransform(pShadowMap->GetShadowTransform());

	//plane
	wnd.GetGraphics().BindVSandIA(DefaultLight_VS_PS);
	wnd.GetGraphics().BindPS(DefaultLight_VS_PS);
	pDC->IASetVertexBuffers(0u, 1u, pPlate->GetVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pPlate->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	wnd.GetGraphics().VSDefaultMatricesUpdate(ID, pPlate->plateScaling, ID);
	wnd.GetGraphics().DefaultLightUpdate(pPlate->plateMaterial, false, 
		usessao, pPlate->diffuseMap, pPlate->normalMap);
	pDC->DrawIndexed(pPlate->GetIndexCount(), 0u, 0u);

	//columns
	pDC->IASetVertexBuffers(0u, 1u, pCylinder->GetVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pCylinder->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	wnd.GetGraphics().DefaultLightUpdate(pCylinder->cylinderMaterial, false,
		usessao, pCylinder->diffuseMap, pCylinder->normalMap);
	for (int i = 0; i < 10; i++)
	{
		wnd.GetGraphics().VSDefaultMatricesUpdate(pCylinder->m_CylWorld[i], ID, ID);
		pDC->DrawIndexed(pCylinder->GetIndexCount(), 0u, 0u);
	}

 	//box
	pDC->IASetVertexBuffers(0u, 1u, pBox->GetVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pBox->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	wnd.GetGraphics().VSDefaultMatricesUpdate(shapes.Get_m_BoxWorld(),ID, ID);
	wnd.GetGraphics().DefaultLightUpdate(pBox->boxMaterial, false,  usessao,
		pBox->diffuseMap, pBox->normalMap);
	pDC->DrawIndexed(pBox->GetIndexCount(), 0u, 0u);

	//skull
	pDC->IASetVertexBuffers(0u, 1u, pSkull->GetVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pSkull->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	wnd.GetGraphics().VSDefaultMatricesUpdate(pSkull->skullWorld, ID, ID);
	wnd.GetGraphics().DefaultLightUpdate(pSkull->skullMaterial, true, usessao,
		pSkull->diffuseMap, pSkull->normalMap);
	pDC->DrawIndexed(pSkull->GetIndexCount(), 0u, 0u);

	//spheres
	pDC->IASetVertexBuffers(0u, 1u, pGeoSphere->GetVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pGeoSphere->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	//set any material, it's not being used for spheres
	wnd.GetGraphics().DefaultLightUpdate(pGeoSphere->sphereMaterial, true, usessao, pGeoSphere->diffuseMap, pGeoSphere->normalMap);
	for (int i = 0; i < 10; i++)
	{
		wnd.GetGraphics().VSDefaultMatricesUpdate(pGeoSphere->m_SphereWorld[i], ID, ID);
		pDC->DrawIndexed(pGeoSphere->GetIndexCount(), 0u, 0u);
	}

	//////////////////////////////////////////////////////////////////////////
	//DEBUG quad
	wnd.GetGraphics().BindVSandIA(DrawDebugTexQuad_VS_PS);
	wnd.GetGraphics().BindPS(DrawDebugTexQuad_VS_PS);
	stride = sizeof(vbPosNormalTex);
	pDC->IASetVertexBuffers(0u, 1u, pSSAO->GetQuadVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pSSAO->GetQuadIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
// 	ID3D11ShaderResourceView* pNMSRV = pSSAO->GetNormalMapSRV();
	ID3D11ShaderResourceView* pNMSRV = pSSAO->GetAmbientMapSRV0();
	pDC->PSSetShaderResources(5u, 1u, &pNMSRV);
	pDC->DrawIndexed(pSSAO->GetQuadIndexCount(), 0u, 0u);
	//////////////////////////////////////////////////////////////////////////
	//release for the SSAO pass
	ID3D11ShaderResourceView* pNullSRV = nullptr;
	pDC->PSSetShaderResources(5u, 1u, &pNullSRV);

	//Displacement waves
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(wnd.GetGraphics().TransparentBS, blendFactorsZero, 0xffffffff);

	pDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	wnd.GetGraphics().BindVSandIA(DisplacementWaves_VS_HS_DS_PS);
	wnd.GetGraphics().BindPS(DisplacementWaves_VS_HS_DS_PS);
	wnd.GetGraphics().BindHS(DisplacementWaves_VS_HS_DS_PS);
	wnd.GetGraphics().BindDS(DisplacementWaves_VS_HS_DS_PS);
	wnd.GetGraphics().SetDispWavesShaderRes(pDispWaves->normalMap0, pDispWaves->normalMap1, pDispWaves->diffuseMap);
	wnd.GetGraphics().UpdateDispWavesCBuffers(pDispWaves->wavesWorld, pDispWaves->wavesMaterial);
	pDC->IASetVertexBuffers(0u, 1u, pDispWaves->GetVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pDispWaves->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	pDC->DrawIndexed(pDispWaves->GetIndexCount(), 0u, 0u);
	wnd.GetGraphics().UnbindAll();

	//Skybox
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(wnd.GetGraphics().noBlendBS, blendFactorsZero, 0xffffffff);

	pDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pDC->RSSetState(wnd.GetGraphics().NoCullRS);
	pDC->OMSetDepthStencilState(wnd.GetGraphics().LessEqualDSS, 0u);

	wnd.GetGraphics().BindVSandIA(ShaderPicker::Sky_VS_PS);
	wnd.GetGraphics().BindPS(ShaderPicker::Sky_VS_PS);
	wnd.GetGraphics().ConstBufferVSMatricesBind();
	wnd.GetGraphics().VSDefaultMatricesUpdate(ID, ID, ID);
	stride = sizeof(DirectX::XMFLOAT3);
	pDC->IASetVertexBuffers(0u, 1u, pSky->GetVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pSky->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	pDC->DrawIndexed(pSky->GetIndexCount(), 0u, 0u);

	pDC->RSSetState(0u);
	pDC->OMSetDepthStencilState(0u, 0u);
	//release shadow map SRV to generate a new one
	ID3D11ShaderResourceView* pNULLSRV = nullptr;
	pDC->PSSetShaderResources(2u, 1u, &pNULLSRV);

	
}

void App::DrawNormalMap(DirectX::XMMATRIX viewProjectionMatrix)
{
	wnd.GetGraphics().ConstBufferNormalMapBind();
	wnd.GetGraphics().BindVSandIA(NormalMap_VS_PS);
	wnd.GetGraphics().BindPS(NormalMap_VS_PS);

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
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(wnd.GetGraphics().WireframeRS);

	viewProjectionMatrix = GetViewProjectionCamera();
	pQuadTess->UpdateTessellationShaderBuffers(wnd.GetGraphics(), viewProjectionMatrix, DirectX::XMMatrixTranslation(0.0f, -25.0f, 0.0f), camera.GetCameraPosition());

// 	wnd.GetGraphics().BindVSandIA(QuadTessellation_VS);
// 	wnd.GetGraphics().BindHS(QuadTessellation_HS);
// 	wnd.GetGraphics().BindDS(QuadTessellation_DS);
// 	wnd.GetGraphics().BindPS(QuadTessellation_PS);
// 	pQuadTess->BindAndDraw(wnd.GetGraphics(), 16u, 0u);
}

void App::InstancingCreate()
{
	pInstancedSkulls = new InstancedSkull(wnd.GetGraphics());
}

void App::DrawInstancingDraw()
{
	viewProjectionMatrix = GetViewProjectionCamera();
// 	wnd.GetGraphics().BindVSandIA(ShaderPicker::InstancedSkull_VS);
// 	wnd.GetGraphics().BindPS(ShaderPicker::InstancedSkull_PS);
	pInstancedSkulls->UpdateVSMatrices(wnd.GetGraphics(), viewProjectionMatrix, camera.GetViewMatrix(), camera.GetProjecion());
	pInstancedSkulls->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
// 	pInstancedSkulls->BindAndDrawInstancedIndexed(wnd.GetGraphics(), pInstancedSkulls->GetAmountOfVisible(), 0u, 0u, 0u);
}




void App::CreateTerrain()
{
  	pTerrain = new Terrain(wnd.GetGraphics());
	wnd.GetGraphics().AddSRVToMap(pTerrain->nameHeightMap, pTerrain->pHeightMapPS, false, true);
	wnd.GetGraphics().AddSRVToMap(pTerrain->nameHeightMap, pTerrain->pHeightMapPS, true, false);
	cbPSerrainTexel terrainPsData = pTerrain->terrainConstants;
	wnd.GetGraphics().CreateRuntimeCBuffers(terrainPsData, cbNames.terrainTexelInfo, "terrain texel data PS");
	pSky = new Sky(wnd.GetGraphics());
	wnd.GetGraphics().BindCubeMap(pSky->skyBoxName);

// 	wnd.GetGraphics().BindCubeMap(pSky->skyBoxName);
   	pParticle = new ParticleSystem(wnd.GetGraphics(), 500);
  	pParticleRain = new ParticleSystem(wnd.GetGraphics(), 6000);
   	pParticleExplosion = new ParticleSystem(wnd.GetGraphics(), 2500);

  	pParticleFountain = new ParticleSystem(wnd.GetGraphics(), 2000);

	wnd.GetGraphics().SetParticleBuffers(
		pParticleRain->GetStreamOutVertexBuffer(),
		pParticleRain->GetDrawVertexBuffer(),
		pParticleRain->GetRandomTexSRV(),
		pParticleRain->GetInitVB(), ParticlePick::Rain);
	wnd.GetGraphics().SetParticleBuffers(
		pParticleExplosion->GetStreamOutVertexBuffer(),
		pParticleExplosion->GetDrawVertexBuffer(),
		pParticleExplosion->GetRandomTexSRV(),
		pParticleExplosion->GetInitVB(), ParticlePick::Explosion);
	wnd.GetGraphics().SetParticleBuffers(
		pParticle->GetStreamOutVertexBuffer(),
		pParticle->GetDrawVertexBuffer(),
		pParticle->GetRandomTexSRV(),
		pParticle->GetInitVB(), ParticlePick::Fire);
	wnd.GetGraphics().SetParticleBuffers(
		pParticleFountain->GetStreamOutVertexBuffer(),
		pParticleFountain->GetDrawVertexBuffer(),
		pParticleFountain->GetRandomTexSRV(),
		pParticleFountain->GetInitVB(), ParticlePick::Fountain);
}


void App::DrawTerrain()
{
	stride = sizeof(vbPosTexBoundsY);
	offset = 0;
	// TERRAIN
	wnd.GetGraphics().UnbindAll();
	wnd.GetGraphics().BindVSandIA(ShaderPicker::TerrainHeightMap);
	wnd.GetGraphics().BindHS(ShaderPicker::TerrainHeightMap);
	wnd.GetGraphics().BindDS(ShaderPicker::TerrainHeightMap);
	wnd.GetGraphics().BindPS(ShaderPicker::TerrainHeightMap);
	wnd.GetGraphics().SetDefaultLightData();

	wnd.GetGraphics().SetTerrainShaderResAndUpdateCbuffers(pTerrain->terrainWorld, pTerrain->blendMap, pTerrain->snow,
		pTerrain->pHeightMapDS, pTerrain->pHeightMapVS);
	wnd.GetGraphics().TerrainLightUpdate(pTerrain->terrainMaterial, false, false);


	//bind camera to terrain surface
// 	DirectX::XMFLOAT3 camPos = camera.GetCameraPosition();
// 	float y = pTerrain->GetHeight(camPos.x, camPos.z, camPos.y);
// 	camera.SetCameraYPosition(y + 2.0f);
	wnd.GetGraphics().pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	if (GetAsyncKeyState('6') & 0x8000)
	{
		wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(wnd.GetGraphics().WireframeRS);
	}

	pDC->IASetVertexBuffers(0u, 1u, pTerrain->GetVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pTerrain->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	pDC->DrawIndexed(pTerrain->GetNumQuadFaces() * 4, 0u, 0u);


	// PARTICLES
	wnd.GetGraphics().pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	wnd.GetGraphics().UnbindAll();

	//RAIN
	DirectX::XMFLOAT3 rainPosition = camera.GetCameraPosition();
	rainPosition.z += 10.0f;
 	rainPosition.y += 5.0f;
	wnd.GetGraphics().DrawParticle(rainPosition, ParticlePick::Rain);
	wnd.GetGraphics().UnbindAll();

	//FOUNTAIN
	DirectX::XMFLOAT3 fountainPos = DirectX::XMFLOAT3(0.0f, 4.0f, -4.0f);
	fountainPos.z += 10.0f;
	wnd.GetGraphics().DrawParticle(fountainPos, ParticlePick::Fountain);
	wnd.GetGraphics().UnbindAll();

	//EXPLOSION
	DirectX::XMFLOAT3 explosionPos = DirectX::XMFLOAT3(-4.0f, 4.0f, -4.0f);
	explosionPos.z += 10.0f;
	wnd.GetGraphics().DrawParticle(explosionPos, ParticlePick::Explosion);
	wnd.GetGraphics().UnbindAll();

	//FIRE
	//supposed to be drawn last so it will blend
	DirectX::XMFLOAT3 firePos = DirectX::XMFLOAT3(0.0f, 4.0f, -4.0f);
	firePos.z += 10.0f;
	wnd.GetGraphics().DrawParticle(firePos, ParticlePick::Fire);
	wnd.GetGraphics().UnbindAll();


		//Skybox
	pDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pDC->RSSetState(wnd.GetGraphics().NoCullRS);
	pDC->OMSetDepthStencilState(wnd.GetGraphics().LessEqualDSS, 0u);

	wnd.GetGraphics().BindVSandIA(ShaderPicker::Sky_VS_PS);
	wnd.GetGraphics().BindPS(ShaderPicker::Sky_VS_PS);
	wnd.GetGraphics().ConstBufferVSMatricesBind();
	wnd.GetGraphics().VSDefaultMatricesUpdate(ID, ID, ID);
	stride = sizeof(DirectX::XMFLOAT3);
	pDC->IASetVertexBuffers(0u, 1u, pSky->GetVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pSky->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	pDC->DrawIndexed(pSky->GetIndexCount(), 0u, 0u);

	pDC->RSSetState(0u);
	pDC->OMSetDepthStencilState(0u, 0u);

	//reset
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(0u, blendFactorsZero, 0xffffffff);
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(0u, 0u);
	wnd.GetGraphics().UnbindAll();
}

App::~App()
{
}
