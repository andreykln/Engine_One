
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

// 	CreateBilateralHillsBlur();
// 	CreateBox();
	CreateShadowMapDemo();
	CreateHillsWithGPUWaves();
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
	DrawHillsWithGPUWaves();
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
	pShaders->UnbindCS();

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
	pBox = new Box(wnd.GetGraphics(), 7.0f, 7.0f, 7.0f, DemoSwitch::DefaultBox);
}

void App::DrawBox()
{
	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::LightAndTexture_VS_PS);
	viewProjectionMatrix = GetViewProjectionCamera();

	DirectX::XMFLOAT4X4 test;
	test._11 = 1.0f;
	test._12 = 0.0f;
	test._13 = 0.0f;
	test._14 = 0.0f;

	test._21 = 0.0f;
	test._22 = 1.0f;
	test._23 = 0.0f;
	test._24 = 0.0f;

	test._31 = 0.0f;
	test._32 = 0.0f;
	test._33 = 1.0f;
	test._34 = 0.0f;

	test._41 = 0.0f;
	test._42 = 0.0f;
	test._43 = 50.0f;
	test._44 = 1.0f;
	
	DirectX::XMMATRIX worldTest = DirectX::XMLoadFloat4x4(&test);
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationX(timer.TotalTime());
	DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(9.0f, 6.0f, 15.0f);

	DirectX::XMMATRIX rotationThenTranlastion = rotation * worldTest;
	DirectX::XMMATRIX translationThenRotation = worldTest * rotation;


// 	pBox->UpdateVSMatrices(wnd.GetGraphics(), rotationThenTranlastion, viewProjectionMatrix);
// 	pBox->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
// 	pBox->BindAndDrawIndexed(wnd.GetGraphics());
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




void App::DrawSceneToShadowMap()
{
	wnd.GetGraphics().pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	viewProjectionMatrix = GetViewProjectionCamera();
	DirectX::XMMATRIX WVP =
		DirectX::XMMatrixTranslation(0.0f, 6.0f, 0.0) * DirectX::XMMatrixScaling(0.3f, 0.3f, 0.3f) *
		shapes.GetCameraOffset() * pShadowMap->GetLighViewProjection();

	pShaders->BindVSandIA(ShaderPicker::ShadowMapGenSkull_VS_PS);
	pShaders->BindPS(ShaderPicker::ShadowMapGenSkull_VS_PS);
	pSkull->UpdateShadomMapGenBuffers(wnd.GetGraphics(),
		WVP, camera.GetCameraPosition());
	pSkull->BindAndDrawIndexed(wnd.GetGraphics());

	pShaders->BindVSandIA(ShaderPicker::ShadowMapInstancedGen_VS);
 	pShaders->BindPS(ShaderPicker::ShadowMapGen_VS_PS);

	//copy in argument
	pShadowMap->BuildShadowTransform(pShadowMap->GetNewLightDirection());

	DirectX::XMMATRIX VP = pShadowMap->GetLighViewProjection();

	pInstancedCylinder->UpdateShadowMapGenBuffersInstanced(wnd.GetGraphics(), VP);
	pInstancedCylinder->BindAndDrawInstancedIndexed(wnd.GetGraphics(), 10, 0u, 0u, 0u);


	pInstancedGeoSphere->UpdateShadowMapGenBuffersInstanced(wnd.GetGraphics(), VP);
	pInstancedGeoSphere->BindAndDrawInstancedIndexed(wnd.GetGraphics(), 10, 0, 0, 0);

	pShaders->BindVSandIA(ShaderPicker::ShadowMapGen_VS_PS);

	pHills->UpdateShadomMapGenBuffers(wnd.GetGraphics(),
		shapes.Get_m_GridWorld() * shapes.GetCameraOffset() * pShadowMap->GetLighViewProjection(), camera.GetCameraPosition());
	pHills->BindAndDrawIndexed(wnd.GetGraphics());

	WVP = shapes.Get_m_BoxWorld() * shapes.GetCameraOffset() * pShadowMap->GetLighViewProjection();

	pDisplacementMappingBox->UpdateShadomMapGenBuffers(wnd.GetGraphics(), WVP, camera.GetCameraPosition());
	pDisplacementMappingBox->BindAndDrawIndexed(wnd.GetGraphics());






}

void App::CreateShadowMapDemo()
{
	pSky = new Sky(wnd.GetGraphics());
	pDisplacementMappingBox = new Box(wnd.GetGraphics(), 1.0f, 1.0f, 2.0f, DemoSwitch::ShadowMap);
 	pSkull = new Skull(wnd.GetGraphics(), L"models\\skull.txt");
	pHills = new Hills(wnd.GetGraphics(), 25.0f, 25.0f, 45, 45);
	pInstancedCylinder = new Cylinder(wnd.GetGraphics(), 0.5f, 0.3f, 3.0f, 20, 20);
	pInstancedGeoSphere = new GeoSphere(wnd.GetGraphics(), 0.5f, 2u, false, DemoSwitch::ShadowMap);


}

void App::DrawShadowMapDemo()
{
	wnd.GetGraphics().pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	viewProjectionMatrix = GetViewProjectionCamera();


	pShadowMap->BindDSVandSetNullRenderTarget(wnd.GetGraphics());
	pShadowMap->UpdateScene(timer.DeltaTime());
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::ShadowMapBiasRS);
	DrawSceneToShadowMap();
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(0u);

	//set default render target and viewport
	ID3D11RenderTargetView* renderTargets[1]{ wnd.GetGraphics().pgfx_RenderTargetView.Get() };
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetRenderTargets(1u, &renderTargets[0], wnd.GetGraphics().pgfx_DepthStencilView.Get());
	wnd.GetGraphics().SetViewport();
	/////
	pShadowMap->SetShadowSampler(wnd.GetGraphics());
	
	//cubemap for specular reflections
	wnd.GetGraphics().pgfx_pDeviceContext->PSSetShaderResources(3u, 1u, pSky->GetSkyCubeMap());


	auto newLightDirection = pShadowMap->GetNewLightDirection();
	pShaders->BindVSandIA(ShaderPicker::ShadowMapDrawSkull_VS_PS);
	pShaders->BindPS(ShaderPicker::ShadowMapDrawSkull_VS_PS);
	pSkull->UpdateShadowMapDrawBuffers(wnd.GetGraphics(), camera.GetCameraPosition(), pShadowMap->GetShadowTransform(),
		shapes.GetCameraOffset() * DirectX::XMMatrixTranslation(0.0f, 6.0f, 0.0f)
		* DirectX::XMMatrixScaling(0.3f, 0.3f, 0.3f), viewProjectionMatrix,
		pShadowMap->DepthMapSRV(), newLightDirection);
	pSkull->BindAndDrawIndexed(wnd.GetGraphics());

	pShaders->BindVSandIA(ShaderPicker::ShadowMapInstancedDraw_VS);
	pShaders->BindPS(ShaderPicker::DefaultInstanced_PS);
	pShaders->BindHS(ShaderPicker::DisplacementMapping_VS_DS_HS);
	pShaders->BindDS(ShaderPicker::DisplacementMapping_VS_DS_HS);


	wnd.GetGraphics().pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	//columns
// 	pInstancedCylinder->UpdateDrawInstancedBuffers(wnd.GetGraphics(), camera.GetCameraPosition(),
// 		pShadowMap->GetShadowTransform(), viewProjectionMatrix,
// 		pShadowMap->DepthMapSRV(), pShadowMap->GetNewLightDirection());
// 	pInstancedCylinder->BindAndDrawInstancedIndexed(wnd.GetGraphics(), 10, 0, 0, 0);

	//box
	pDisplacementMappingBox->UpdateShadowMapDrawBuffers(wnd.GetGraphics(), camera.GetCameraPosition(),
		pShadowMap->GetShadowTransform(), shapes.Get_m_BoxWorld() * shapes.GetCameraOffset(), viewProjectionMatrix,
		pShadowMap->DepthMapSRV(), pShadowMap->GetNewLightDirection());
	pDisplacementMappingBox->BindAndDrawInstancedIndexed(wnd.GetGraphics(), 1u, 0, 0, 0);

	pShaders->UnbindDS();
	pShaders->UnbindHS();
	wnd.GetGraphics().pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//spheres
	pInstancedGeoSphere->UpdateShadowMapDrawInstancedBuffers(wnd.GetGraphics(), camera.GetCameraPosition(),
		pShadowMap->GetShadowTransform(), viewProjectionMatrix,
		pShadowMap->DepthMapSRV(), pShadowMap->GetNewLightDirection());
	pInstancedGeoSphere->BindAndDrawInstancedIndexed(wnd.GetGraphics(), 10, 0, 0, 0);


	//plane
	pShaders->BindVSandIA(ShaderPicker::ShadowMap_VS_PS);
	pHills->UpdateShadowMapDrawBuffers(wnd.GetGraphics(), camera.GetCameraPosition(),
		pShadowMap->GetShadowTransform(), shapes.Get_m_GridWorld() * shapes.GetCameraOffset(),
		viewProjectionMatrix, pShadowMap->DepthMapSRV(), pShadowMap->GetNewLightDirection());
	pShadowMap->SetShadowSampler(wnd.GetGraphics());
	pHills->BindAndDrawIndexed(wnd.GetGraphics());




	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::NoCullRS);
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(RenderStates::LessEqualDSS, 0u);

	pShaders->BindVSandIA(ShaderPicker::Sky_VS_PS);
	pShaders->BindPS(ShaderPicker::Sky_VS_PS);
	pSky->DrawSky(wnd.GetGraphics(), viewProjectionMatrix);

	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(0u);
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(0u, 0u);
	//release shadow map srv
	ID3D11ShaderResourceView* pNULLSRV = nullptr;
	wnd.GetGraphics().pgfx_pDeviceContext->PSSetShaderResources(2u, 1u, &pNULLSRV);

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
