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


// 	CreateShadowMapDemo();
// 	CreateComputeShaderWaves();
// 	CreateTerrain();
	CreateTempleScene();


	CreateAndBindSkybox();
}

void App::DoFrame()
{
	viewProjectionMatrix = GetViewProjectionCamera();

// 	const float c = abs((sin(timer.TotalTime())));
	timer.Tick();
	wnd.GetGraphics().SetCommonShaderConstants(viewProjectionMatrix, camera.GetViewMatrix(),
		camera.GetProjecion(), pShadowMap->GetLighViewProjection() ,camera.GetCameraPosition(), timer.DeltaTime(), timer.TotalTime());

// 	DrawShadowMapDemo();
// 	DrawComputeShaderWaves();
// 	DrawTerrain();
	DrawTempleScene();


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





void App::CreateComputeShaderWaves()
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

void App::DrawComputeShaderWaves()
{



// 	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(wnd.GetGraphics().WireframeRS);
// 	viewProjectionMatrix = GetViewProjectionCamera();
	wnd.GetGraphics().SetDefaultLightData();
	//////////////////////////////////////////////////////////////////////////
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
	pBox = new Box(wnd.GetGraphics(), 1.0f, 1.0f, 2.0f);
 	pSkull = new Skull(wnd.GetGraphics(), L"models\\skull.txt");
	pPlate = new Hills(wnd.GetGraphics(), 25.0f, 25.0f, 45, 45);
	pCylinder = new Cylinder(wnd.GetGraphics(), 0.5f, 0.3f, 3.0f, 20, 20);
	pGeoSphere = new GeoSphere(wnd.GetGraphics(), 0.5f, 2u);
	pDispWaves = new DisplacementWaves(wnd.GetGraphics());
}

void App::DrawShadowMapDemo()
{
	pDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	//update every frame
	viewProjectionMatrix = GetViewProjectionCamera();
// 	wnd.GetGraphics().SetCommonShaderConstants(viewProjectionMatrix, camera.GetViewMatrix(),
// 		camera.GetProjecion(), camera.GetCameraPosition(), timer.TotalTime());

	//shadow map
	wnd.GetGraphics().BindVSandIA(ShaderPicker::ShadowMap_VS_PS);
	wnd.GetGraphics().BindPS(ShaderPicker::ShadowMap_VS_PS);
	pShadowMap->BindDSVandSetNullRenderTarget(wnd.GetGraphics());
	pShadowMap->UpdateScene(timer.DeltaTime());
	pDC->RSSetState(wnd.GetGraphics().ShadowMapBiasRS);
	DrawSceneToShadowMap();
	pDC->RSSetState(0u);

	//create normal-depth map
	pSSAO->SetNormalDepthRenderTarget(wnd.GetGraphics(), wnd.GetGraphics().pgfx_DepthStencilView.Get());
	DrawNormalMap();

	//SSAO
	wnd.GetGraphics().BindVSandIA(ShaderPicker::ComputeSSAO_VS_PS);
	wnd.GetGraphics().BindPS(ShaderPicker::ComputeSSAO_VS_PS);
	stride = sizeof(vbPosNormalTex);
	pDC->IASetVertexBuffers(0u, 1u, pSSAO->GetQuadVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pSSAO->GetQuadIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	wnd.GetGraphics().ComputeSSAO(pSSAO->GetAmbientMapRTV0(), pSSAO->GetSSAOViewport(), 
		pSSAO->GetRandomVectorSRV(), pSSAO->GetNormalMapSRV());
	pDC->DrawIndexed(pSSAO->GetQuadIndexCount(), 0u, 0u);

	// blur
	wnd.GetGraphics().BindVSandIA(ShaderPicker::SSAOBlur_VS_PS);
	wnd.GetGraphics().BindPS(ShaderPicker::SSAOBlur_VS_PS);
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
	wnd.GetGraphics().BindVSandIA(ShaderPicker::DefaultLight_VS_PS);
	wnd.GetGraphics().BindPS(ShaderPicker::DefaultLight_VS_PS);
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
// 	wnd.GetGraphics().BindVSandIA(DrawDebugTexQuad_VS_PS);
// 	wnd.GetGraphics().BindPS(DrawDebugTexQuad_VS_PS);
// 	stride = sizeof(vbPosNormalTex);
// 	pDC->IASetVertexBuffers(0u, 1u, pSSAO->GetQuadVertexBuffer(), &stride, &offset);
// 	pDC->IASetIndexBuffer(pSSAO->GetQuadIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
// // 	ID3D11ShaderResourceView* pNMSRV = pSSAO->GetNormalMapSRV();
// 	ID3D11ShaderResourceView* pNMSRV = pSSAO->GetAmbientMapSRV0();
// 	pDC->PSSetShaderResources(5u, 1u, &pNMSRV);
// 	pDC->DrawIndexed(pSSAO->GetQuadIndexCount(), 0u, 0u);
	//////////////////////////////////////////////////////////////////////////
	//release for the SSAO pass
	ID3D11ShaderResourceView* pNullSRV = nullptr;
	pDC->PSSetShaderResources(5u, 1u, &pNullSRV);

	//Displacement waves
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(wnd.GetGraphics().TransparentBS, blendFactorsZero, 0xffffffff);

	pDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	wnd.GetGraphics().BindVSandIA(ShaderPicker::DisplacementWaves_VS_HS_DS_PS);
	wnd.GetGraphics().BindPS(ShaderPicker::DisplacementWaves_VS_HS_DS_PS);
	wnd.GetGraphics().BindHS(ShaderPicker::DisplacementWaves_VS_HS_DS_PS);
	wnd.GetGraphics().BindDS(ShaderPicker::DisplacementWaves_VS_HS_DS_PS);
	wnd.GetGraphics().SetDispWavesShaderRes(pDispWaves->normalMap0, pDispWaves->normalMap1, pDispWaves->diffuseMap);
	wnd.GetGraphics().UpdateDispWavesCBuffers(pDispWaves->wavesWorld, pDispWaves->wavesMaterial);
	pDC->IASetVertexBuffers(0u, 1u, pDispWaves->GetVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pDispWaves->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	pDC->DrawIndexed(pDispWaves->GetIndexCount(), 0u, 0u);
	wnd.GetGraphics().UnbindAll();


	DrawSkyBox();
	wnd.GetGraphics().ReleaseSSAOShaderResource();

	
}

void App::DrawNormalMap()
{
	wnd.GetGraphics().ConstBufferNormalMapBind();
	wnd.GetGraphics().BindVSandIA(ShaderPicker::NormalMap_VS_PS);
	wnd.GetGraphics().BindPS(ShaderPicker::NormalMap_VS_PS);

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

void App::CreateTempleScene()
{
	m3dLoad = new M3dLoader("models\\base.m3d");
	M3dRawData d = m3dLoad->rawData;
	delete m3dLoad;
	m3dLoad = nullptr;
	wnd.GetGraphics().CreateM3dModel(d);



	pSky = new Sky(wnd.GetGraphics());
	wnd.GetGraphics().BindCubeMap(pSky->skyBoxName);

}

void App::DrawTempleScene()
{
	pDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	std::vector<DirectX::XMMATRIX> templebaseWorld(3);
	templebaseWorld[0] = DirectX::XMMatrixIdentity();
	templebaseWorld[1] = DirectX::XMMatrixIdentity();
	templebaseWorld[2] = DirectX::XMMatrixIdentity();


	DirectX::XMMATRIX w = DirectX::XMMatrixIdentity();

	////	//shadow map
	wnd.GetGraphics().BindVSandIA(ShaderPicker::ShadowMap_VS_PS);
	wnd.GetGraphics().BindPS(ShaderPicker::ShadowMap_VS_PS);

	pShadowMap->BuildShadowTransform(pShadowMap->GetNewLightDirection());

// 	DirectX::XMMATRIX VP = pShadowMap->GetLighViewProjection();
	wnd.GetGraphics().ConstBufferShadowMapBind();
// 	wnd.GetGraphics().ShadowMap(w, VP);


	pShadowMap->BindDSVandSetNullRenderTarget(wnd.GetGraphics());
	pShadowMap->UpdateScene(timer.DeltaTime());
	pDC->RSSetState(wnd.GetGraphics().ShadowMapBiasRS);
	wnd.GetGraphics().DrawM3dStaticModel(m3dNames.templeBase, Technique::ShadowMap, templebaseWorld);
	pDC->RSSetState(0u);

	//create normal-depth map
	pSSAO->SetNormalDepthRenderTarget(wnd.GetGraphics(), wnd.GetGraphics().pgfx_DepthStencilView.Get());
	wnd.GetGraphics().ConstBufferNormalMapBind();

	wnd.GetGraphics().BindVSandIA(ShaderPicker::NormalMap_VS_PS);
	wnd.GetGraphics().BindPS(ShaderPicker::NormalMap_VS_PS);

// 	wnd.GetGraphics().NormalMap(pSkull->skullWorld);
	wnd.GetGraphics().DrawM3dStaticModel(m3dNames.templeBase, Technique::NormalMap, templebaseWorld);

	//SSAO
	wnd.GetGraphics().BindVSandIA(ShaderPicker::ComputeSSAO_VS_PS);
	wnd.GetGraphics().BindPS(ShaderPicker::ComputeSSAO_VS_PS);
	stride = sizeof(vbPosNormalTex);
	pDC->IASetVertexBuffers(0u, 1u, pSSAO->GetQuadVertexBuffer(), &stride, &offset);
	pDC->IASetIndexBuffer(pSSAO->GetQuadIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	wnd.GetGraphics().ComputeSSAO(pSSAO->GetAmbientMapRTV0(), pSSAO->GetSSAOViewport(),
		pSSAO->GetRandomVectorSRV(), pSSAO->GetNormalMapSRV());
	pDC->DrawIndexed(pSSAO->GetQuadIndexCount(), 0u, 0u);

	// blur
	wnd.GetGraphics().BindVSandIA(ShaderPicker::SSAOBlur_VS_PS);
	wnd.GetGraphics().BindPS(ShaderPicker::SSAOBlur_VS_PS);
	wnd.GetGraphics().BlurSSAOMap(4, pSSAO->GetAmbientMapRTV0(), pSSAO->GetAmbientMapRTV1(), pSSAO->GetAmbientMapSRV0(),
		pSSAO->GetAmbientMapSRV1(), pSSAO->GetSSAOViewport());
	wnd.GetGraphics().UnbindVS();
	wnd.GetGraphics().UnbindPS();
	wnd.GetGraphics().ReleaseNormalMapResource();
	SetDefaultRTVAndViewPort();


	pDC->PSSetShaderResources(2u, 1u, pShadowMap->DepthMapSRV());
	bool usessao = true;
	if (GetAsyncKeyState('5') & 0x8000)
		usessao = false;
	else
		usessao = true;
	wnd.GetGraphics().SetDefaultLightData();
	wnd.GetGraphics().UpdateLightDirection(pShadowMap->GetNewLightDirection());
	wnd.GetGraphics().SetShadowTransform(pShadowMap->GetShadowTransform());


	//////////////////////////////////////////////////////////////////////////
	wnd.GetGraphics().SetDefaultLightData();
	wnd.GetGraphics().ConstBufferVSMatricesBind();

	wnd.GetGraphics().BindVSandIA(ShaderPicker::DefaultLight_VS_PS);
	wnd.GetGraphics().BindPS(ShaderPicker::DefaultLight_VS_PS);

	wnd.GetGraphics().DrawM3dStaticModel(m3dNames.templeBase, Technique::DefaultLight, templebaseWorld);



	DrawSkyBox();
	ID3D11ShaderResourceView* nullsrv = nullptr;
	pDC->PSSetShaderResources(2u, 1u, &nullsrv);
	wnd.GetGraphics().ReleaseSSAOShaderResource();
}

void App::CreateAndBindSkybox()
{
	pSky = new Sky(wnd.GetGraphics());
	wnd.GetGraphics().BindCubeMap(pSky->skyBoxName);
}

void App::DrawSkyBox()
{
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

}

App::~App()
{
}
