
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "App.h"
App::App()
	: wnd("Output Window", resolution_width, resolution_height)
{
	rStates.InitializeAll(wnd.GetGraphics());
	pShaders = new Shaders(wnd.GetGraphics());

// 	CreateBilateralHillsBlur();
// 	CreateBox();
// 	CreateShapesWithDynamicCubeMap();
	CreateShadowMapDemo();
// 	CreateHillsWithGPUWaves();
// 	CreateMirror();
// 	CreateDepthComplexityStencil();
// 	CreateGaussBlur();
// 	CreateBezierPatchTess();
// 	CreatePicking();
// 	CreateTerrain();

}

void App::DoFrame()
{

// 	const float c = abs((sin(timer.TotalTime())));
	timer.Tick();
// 	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(RenderStates::NoRenderTargetWritesBS, blendFactorsZero, 0xffffffff);
// 
// 	DrawShapesWithDynamicCubeMap();
	DrawShadowMapDemo();
// 	DrawMirror();
// 	DrawHillsWithGPUWaves();
// 	DrawBox();
// 	DrawDepthComplexityStencil();
// 	DrawGaussBlur();
// 	DrawBilateralHillsBlur();
// 	DrawBezierPatchTess();
// 	DrawPicking();
// 	DrawTerrain();


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
	pHills = new Hills(wnd.GetGraphics(), 160.0f, 160.0f, 50u, 50u, DemoSwitch::HillsAllLight);
	pBox = new Box(wnd.GetGraphics(), 5.0f, 5.0f, 5.0f, DemoSwitch::HillsDemo);
	pBillboards = new TreeBillboard(wnd.GetGraphics());

}

void App::DrawHillsWithGPUWaves()
{
// 	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::WireframeRS);
	viewProjectionMatrix = GetViewProjectionCamera();
	wnd.GetGraphics().pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::HillsAllLight_PS);
	pHills->UpdateVSMatrices(wnd.GetGraphics(), pHills->GetHillsOffset(), viewProjectionMatrix, camera.GetCameraPosition());
	pHills->UpdatePSAllLights(wnd.GetGraphics(), camera.GetCameraPosition(), camera.GetCameraDirection(), timer.TotalTime());
	pHills->BindAndDrawIndexed(wnd.GetGraphics());

	//billboard
	pShaders->BindVSandIA(ShaderPicker::TreeBillboardVS_PS_GS);
	pShaders->BindGS(ShaderPicker::TreeBillboardVS_PS_GS);
	pShaders->BindPS(ShaderPicker::TreeBillboardVS_PS_GS);
	pBillboards->UpdateConstantBuffers(wnd.GetGraphics(), viewProjectionMatrix, camera.GetCameraPosition());
	pBillboards->BindAndDraw(wnd.GetGraphics(), 25u, 0u);
	pShaders->UnbindGS();

	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::LightAndTexture_VS_PS);
	//for drawing backside of the wire box
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::NoCullRS);
	pBox->UpdateVSMatrices(wnd.GetGraphics(), pBox->GetBoxForHillsOffset(), viewProjectionMatrix);
	pBox->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());;
	pBox->BindAndDrawIndexed(wnd.GetGraphics());

	//GPU waves
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
	//in order to avoid binding every frame
	if (picker == ShaderPicker::LightAndTexture_VS_PS)
	{
		pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
		pShaders->BindPS(ShaderPicker::LightAndTexture_VS_PS);
		picker = ShaderPicker::Keep;
	}
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


	pBox->UpdateVSMatrices(wnd.GetGraphics(), rotationThenTranlastion, viewProjectionMatrix);
	pBox->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
	pBox->BindAndDrawIndexed(wnd.GetGraphics());
}

void App::CreateMirror()
{
	pBox = new Box(wnd.GetGraphics(), 2.5f, 2.5f, 1.5f, DemoSwitch::Shapesdemo);
	pSkull = new Skull(wnd.GetGraphics(), L"models\\skull.txt", DemoSwitch::MirrorSkull);
	pMirrorRoom = new MirrorRoom(wnd.GetGraphics());
}

void App::DrawMirror()
{
	viewProjectionMatrix = GetViewProjectionCamera();
	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::MirrorRoomPS);


	// 	//restore the material state
	pSkull->UpdateMaterial(wnd.GetGraphics(), false);

	//floor
	pMirrorRoom->UpdateVSMatrices(wnd.GetGraphics(), DirectX::XMMatrixIdentity(), viewProjectionMatrix);
	pMirrorRoom->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
	pMirrorRoom->SwitchTexture(wnd.GetGraphics(), 2u);
	pMirrorRoom->BindAndDraw(wnd.GetGraphics(), 6u, 0u);
	//wall
	pMirrorRoom->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
	pMirrorRoom->SwitchTexture(wnd.GetGraphics(), 0u);
	pMirrorRoom->BindAndDraw(wnd.GetGraphics(), 18u, 6u);

	//rotate reflection and the original
//  	mirroredSkull = DirectX::XMMatrixRotationY(abs((sin(timer.DeltaTime())))) * mirroredSkull;
	pShaders->BindVSandIA(ShaderPicker::Light_VS_PS);
	pShaders->BindPS(ShaderPicker::MirrorSkull_PS);
	pSkull->UpdateVSMatrices(wnd.GetGraphics(), pSkull->GetMirroredSkullTranslation() * DirectX::XMMatrixScaling(0.3f, 0.3f, 0.3f), viewProjectionMatrix);
	pSkull->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
	pSkull->BindAndDrawIndexed(wnd.GetGraphics());
	
	// Do not write to render target
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(RenderStates::NoRenderTargetWritesBS, blendFactorsZero, 0xffffffff);
	// Render visible mirror pixels to stencil buffer.
	// Do not write mirror depth to depth buffer at this point, otherwise it will occlude the reflection.
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(RenderStates::MarkMirrorDSS, 1);
	// draw mirror
	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::MirrorRoomPS);
	pMirrorRoom->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
	pMirrorRoom->SwitchTexture(wnd.GetGraphics(), 1u);
	pMirrorRoom->UpdateVSMatrices(wnd.GetGraphics(), DirectX::XMMatrixIdentity(), viewProjectionMatrix);
	pMirrorRoom->BindAndDraw(wnd.GetGraphics(), 6u, 24u);
	//restore states
 	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(0, 0);
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(0, blendFactorsZero, 0xffffffff);

	
	//skull reflection
	DirectX::XMVECTOR mirrorPlane = {0.0f, 0.0f, 1.0f, 0.0f};
	DirectX::XMMATRIX R = DirectX::XMMatrixReflect(mirrorPlane);

	//cache the old light direction, and reflect light about mirror plane
	DirectX::XMFLOAT3 oldLIghtDirection[3];
	ZeroMemory(&oldLIghtDirection, sizeof(DirectX::XMFLOAT3[3]));
	DirectX::XMFLOAT3 reflectedLightDirection;
	ZeroMemory(&reflectedLightDirection, sizeof(DirectX::XMFLOAT3));
	DirectX::XMFLOAT3 currentLightDirection;
	ZeroMemory(&currentLightDirection, sizeof(DirectX::XMFLOAT3));
	DirectX::XMVECTOR lightDirtemp;
	ZeroMemory(&lightDirtemp, sizeof(DirectX::XMVECTOR));
	DirectX::XMVECTOR reflectedLD;
	ZeroMemory(&reflectedLD, sizeof(DirectX::XMVECTOR));
	UINT numOfLights{ 3 };
	for (UINT i = 0; i < numOfLights; i++)
	{
		oldLIghtDirection[i] = pSkull->GetLightDirection(i).direction;
		currentLightDirection = pSkull->GetLightDirection(i).direction;
		lightDirtemp = DirectX::XMLoadFloat3(&currentLightDirection);
		reflectedLD = DirectX::XMVector3TransformNormal(lightDirtemp, R);
		DirectX::XMStoreFloat3(&reflectedLightDirection, reflectedLD);
		pSkull->UpdatePSLightDirection(wnd.GetGraphics(), reflectedLightDirection, i);
	}


	//cull clockwise triangles for reflections.
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::CullClockwiseRS);

	// Only draw reflection into visible mirror pixels as marked by the stencil buffer. 
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(RenderStates::DrawReflectionDSS, 1);
	//draw reflected floor
	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::MirrorRoomPS);
	pMirrorRoom->UpdateVSMatrices(wnd.GetGraphics(), R, viewProjectionMatrix);
	pMirrorRoom->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
	pMirrorRoom->SwitchTexture(wnd.GetGraphics(), 2u);
	pMirrorRoom->BindAndDraw(wnd.GetGraphics(), 6u, 0u);
	//draw reflected skull
	pShaders->BindVSandIA(ShaderPicker::Light_VS_PS);
	pShaders->BindPS(ShaderPicker::MirrorSkull_PS);
	pSkull->UpdateVSMatrices(wnd.GetGraphics(), pSkull->GetMirroredSkullTranslation() * R * DirectX::XMMatrixScaling(0.3f, 0.3f, 0.3f), viewProjectionMatrix);
	pSkull->BindAndDrawIndexed(wnd.GetGraphics());
	// Restore default states.
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(0);
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(0, 0);

	//restore light direction
	for (UINT i = 0; i < numOfLights; ++i)
	{
		pSkull->UpdatePSLightDirection(wnd.GetGraphics(), oldLIghtDirection[i], i);
	}

	// Draw the mirror to the back buffer as usual but with transparency
	// blending so the reflection shows through.
	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::MirrorRoomPS);
	pMirrorRoom->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
	pMirrorRoom->SwitchTexture(wnd.GetGraphics(), 1u);
	pMirrorRoom->UpdateVSMatrices(wnd.GetGraphics(), DirectX::XMMatrixIdentity(), viewProjectionMatrix);
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(RenderStates::TransparentBS, blendFactorsZero, 0xffffffff);
	pMirrorRoom->BindAndDraw(wnd.GetGraphics(), 6u, 24u);


	//shadow
	pShaders->BindVSandIA(ShaderPicker::Light_VS_PS);
	pShaders->BindPS(ShaderPicker::MirrorSkull_PS);
	DirectX::XMVECTOR shadowPlane = DirectX::XMVectorSet(0.0, 1.0f, 0.0, 0.0f); //xz plane
	DirectX::XMFLOAT3 toMainLightTemp = pSkull->GetLightDirection(0).direction;
	using namespace DirectX; //for - sign
	DirectX::XMVECTOR toMainLight = -DirectX::XMLoadFloat3(&toMainLightTemp);
	DirectX::XMMATRIX S = DirectX::XMMatrixShadow(shadowPlane, toMainLight);
	DirectX::XMMATRIX shadowOffsetY = DirectX::XMMatrixTranslation(0.0f, 0.001f, 0.0f);
	pSkull->UpdateMaterial(wnd.GetGraphics(), true);

	pSkull->UpdateVSMatrices(wnd.GetGraphics(), pSkull->GetMirroredSkullTranslation() * S * shadowOffsetY * DirectX::XMMatrixScaling(0.3f, 0.3f, 0.3f), viewProjectionMatrix);

	pSkull->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(RenderStates::NoDoubleBlendDSS, 0);
	pSkull->BindAndDrawIndexed(wnd.GetGraphics());
	// Restore default states.
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(0);
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(0, 0);



}


void App::DrawDepthComplexityStencil()
{
	wnd.GetGraphics().pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	viewProjectionMatrix = GetViewProjectionCamera();
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(RenderStates::DepthComplexityCountDSS, 0);
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::CullCounterClockwiseRS);


	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::LightAndTexture_VS_PS);

	pHills->UpdateVSMatrices(wnd.GetGraphics(), pHills->GetHillsOffset(), viewProjectionMatrix, camera.GetCameraPosition());
	pHills->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
	pHills->BindAndDrawIndexed(wnd.GetGraphics());

// disable transparency for overdraw counting
// 	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(RenderStates::TransparentBS, blendFactorsZero, 0xffffffff);
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::NoCullRS);
// 
	pBox->UpdateVSMatrices(wnd.GetGraphics(), pBox->GetBoxForHillsOffset(), viewProjectionMatrix);
	pBox->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
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
	pHills = new Hills(wnd.GetGraphics(), 160.0f, 160.0f, 50u, 50u, DemoSwitch::HillsDemo);
	pBox = new Box(wnd.GetGraphics(), 5.0f, 5.0f, 5.0f, DemoSwitch::HillsDemo);

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


void App::CreateShapesWithDynamicCubeMap()
{
	pDisplacementWaves = new DisplacementWaves(wnd.GetGraphics());
	pSky = new Sky(wnd.GetGraphics());
	pDynamicCubeMap = new DynamicCubeMap(wnd.GetGraphics());
	pBox = new Box(wnd.GetGraphics(), 1.5f, 1.5f, 2.5f, DemoSwitch::Shapesdemo);
	pDisplacementMappingBox = new Box(wnd.GetGraphics(), 1.5f, 1.5f, 2.5f, DemoSwitch::DisplacementMapping);
	pGeoSphere = new GeoSphere(wnd.GetGraphics(), 1.5f, 20u, true);
	pSkull = new Skull(wnd.GetGraphics(), L"models\\skull.txt", DemoSwitch::Shapesdemo);
	pHills = new Hills(wnd.GetGraphics(), 25.0f, 25.0f, 65, 45, DemoSwitch::Shapesdemo);
	for (int i = 0; i < 10; i++)
	{
		cylinders.push_back(new Cylinder(wnd.GetGraphics(), 0.5f, 0.3f, 3.0f, 20, 20, DemoSwitch::Shapesdemo));
	}
	for (int i = 0; i < 10; i++)
	{
		displacementCylinders.push_back(new Cylinder(wnd.GetGraphics(), 0.5f, 0.3f, 3.0f, 20, 20, DemoSwitch::DisplacementMapping));
	}
	for (size_t i = 0; i < 10; i++)
	{
		geoSpheres.push_back(new GeoSphere(wnd.GetGraphics(), 0.5f, 2u, false));
	}
}

void App::DrawShapesWithDynamicCubeMap()
{
	ID3D11ShaderResourceView* pNULLSrv = nullptr;
	const float color[] = { 0.69f, 0.77f, 0.87f, 1.0f };
	wnd.GetGraphics().pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	viewProjectionMatrix = GetViewProjectionCamera();

	pDynamicCubeMap->BuildCubeFaceCamera(0.0f, 0.0f, 0.0f);
	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::LightAndTexture_VS_PS);
	ID3D11RenderTargetView* renderTargets[1];

	//generate CubeMaps
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetViewports(1u, &pDynamicCubeMap->GetViewPort());
	for (int i = 0; i < 6; ++i)
	{
		//clear cubemap face and depth buffer
		wnd.GetGraphics().pgfx_pDeviceContext->ClearRenderTargetView(pDynamicCubeMap->pDynamicCubeMapRTV[i], color);
		wnd.GetGraphics().pgfx_pDeviceContext->ClearDepthStencilView(pDynamicCubeMap->GetCubeMapDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		//bind cube map face as render target
		renderTargets[0] = pDynamicCubeMap->pDynamicCubeMapRTV[i];
		wnd.GetGraphics().pgfx_pDeviceContext->OMSetRenderTargets(1, renderTargets, pDynamicCubeMap->GetCubeMapDSV());

		//Camera from textbook, keep it just in case
// 		DirectX::XMMATRIX view = pDynamicCubeMap->DCcamera[i].View();
// 		DirectX::XMMATRIX proj = pDynamicCubeMap->DCcamera[i].Proj();
// 		DirectX::XMMATRIX viewProj = pDynamicCubeMap->DCcamera[i].ViewProj();

		DirectX::XMMATRIX viewProj = pDynamicCubeMap->cubeFaceProjection[i];


		DrawShapesWithoutCenterSphere(viewProj);

	}
	//restore default viewport and RTV
	wnd.GetGraphics().SetViewport();
	renderTargets[0] = wnd.GetGraphics().pgfx_RenderTargetView.Get();
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetRenderTargets(1, renderTargets, wnd.GetGraphics().pgfx_DepthStencilView.Get());

	//have hardware generate lower mipmap levels of cube map
	wnd.GetGraphics().pgfx_pDeviceContext->GenerateMips(pDynamicCubeMap->GetCubeMapSRV());
	//draw scene as normal with center sphere
	wnd.GetGraphics().pgfx_pDeviceContext->ClearRenderTargetView(wnd.GetGraphics().pgfx_RenderTargetView.Get(), color);
	wnd.GetGraphics().pgfx_pDeviceContext->ClearDepthStencilView(wnd.GetGraphics().pgfx_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	

	pShaders->BindVSandIA(ShaderPicker::Light_VS_PS);
	pShaders->BindPS(ShaderPicker::Light_VS_PS);
	DirectX::XMMATRIX skullOrbiting;
	DirectX::XMMATRIX axisRotation;
	axisRotation = DirectX::XMMatrixRotationY(timer.TotalTime());

	skullOrbiting = axisRotation * shapes.Get_m_CenterSphere() * shapes.GetCameraOffset() * DirectX::XMMatrixTranslation(25.0f, 0.0f, 0.0f)
		* DirectX::XMMatrixRotationY(timer.TotalTime());
	pSkull->UpdateVSMatrices(wnd.GetGraphics(), skullOrbiting * DirectX::XMMatrixScaling(0.1f, 0.1f, 0.1f), viewProjectionMatrix);
	pSkull->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
	pSkull->BindAndDrawIndexed(wnd.GetGraphics());

	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::CubeMap_PS);

	//generated cubemap SRV
	wnd.GetGraphics().pgfx_pDeviceContext->PSSetShaderResources(0u, 1u, &pDynamicCubeMap->pDynamicCubeMapSRV);
	pGeoSphere->UpdateVSMatrices(wnd.GetGraphics(),
		DirectX::XMMatrixIdentity(),
		viewProjectionMatrix, 0.0f);
	pGeoSphere->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
	pGeoSphere->BindAndDrawIndexed(wnd.GetGraphics());
	wnd.GetGraphics().pgfx_pDeviceContext->PSSetShaderResources(0u, 1u, &pNULLSrv);


	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::LightAndTexture_VS_PS);

	if (GetAsyncKeyState('7') & 0x8000)
	{
		wnd.GetGraphics().pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
		pShaders->BindVSandIA(ShaderPicker::DisplacementMapping_VS_DS_HS);
		pShaders->BindHS(ShaderPicker::DisplacementMapping_VS_DS_HS);
		pShaders->BindDS(ShaderPicker::DisplacementMapping_VS_DS_HS);
		pShaders->BindPS(ShaderPicker::DisplacementMapping_VS_DS_HS);
	}
	if (GetAsyncKeyState('6') & 0x8000)
	{
		wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::WireframeRS);
	}





	for (auto& x : displacementCylinders)
	{
		x->UpdateDisplacementCBuffers(wnd.GetGraphics(), *(shapes.GetCylinderWorldArray())++ * shapes.GetCameraOffset(),
			viewProjectionMatrix, camera.GetCameraPosition());
		x->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
		x->BindAndDrawIndexed(wnd.GetGraphics());

	}
	shapes.GetCylinderWorldArray() -= 10; //reset array position

	pHills->UpdateVSMatrices(wnd.GetGraphics(), shapes.Get_m_GridWorld() * shapes.GetCameraOffset(),
		viewProjectionMatrix, camera.GetCameraPosition());
	pHills->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
	pHills->BindAndDrawIndexed(wnd.GetGraphics());

	pDisplacementMappingBox->UpdateDisplacementCBuffers(wnd.GetGraphics(),
		shapes.Get_m_BoxWorld() * shapes.GetCameraOffset(), viewProjectionMatrix, camera.GetCameraPosition());
	pDisplacementMappingBox->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
	pDisplacementMappingBox->BindAndDrawIndexed(wnd.GetGraphics());
	pShaders->UnbindDS();
	pShaders->UnbindHS();

	wnd.GetGraphics().pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);


	pShaders->BindVSandIA(ShaderPicker::DisplacementWaves_VS_HS_DS_PS);
	pShaders->BindHS(ShaderPicker::DisplacementWaves_VS_HS_DS_PS);
	pShaders->BindDS(ShaderPicker::DisplacementWaves_VS_HS_DS_PS);
	pShaders->BindPS(ShaderPicker::DisplacementWaves_VS_HS_DS_PS);
	wnd.GetGraphics().pgfx_pDeviceContext->PSSetShaderResources(3u, 1u, pSky->GetSkyCubeMap());

	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(RenderStates::TransparentBS, blendFactorsZero, 0xffffffff);

	pDisplacementWaves->UpdateCBs(wnd.GetGraphics(), shapes.Get_m_GridWorld()* shapes.GetCameraOffset(),
		viewProjectionMatrix, camera.GetCameraPosition(), timer.DeltaTime());
	pDisplacementWaves->BindAndDrawIndexed(wnd.GetGraphics());
	pShaders->UnbindDS();
	pShaders->UnbindHS();
	pShaders->UnbindVS();
	pShaders->UnbindPS();

	//unbind for reusing in generation new cubemap
	wnd.GetGraphics().pgfx_pDeviceContext->PSSetShaderResources(3u, 1u, &pNULLSrv);




	wnd.GetGraphics().pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::LightAndTexture_VS_PS);

	for (auto& x : geoSpheres)
	{
		x->UpdateVSMatrices(wnd.GetGraphics(),
			*(shapes.GetSphereWorldArray())++ * shapes.GetCameraOffset(), viewProjectionMatrix, sin(timer.TotalTime()));
		x->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
		x->BindAndDrawIndexed(wnd.GetGraphics());
	}
	shapes.GetSphereWorldArray() -= 10; //reset array position


	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::NoCullRS);
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(RenderStates::LessEqualDSS, 0u);
	pShaders->BindVSandIA(ShaderPicker::Sky_VS_PS);
	pShaders->BindPS(ShaderPicker::Sky_VS_PS);

	pSky->UpdateVSMatricesAndCubeMap(wnd.GetGraphics(), viewProjectionMatrix);
	pSky->BindAndDrawIndexed(wnd.GetGraphics());

	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(0u);
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(0u, 0u);
}

void App::DrawShapesWithoutCenterSphere(DirectX::XMMATRIX& cubeFaceVP)
{
	DirectX::XMMATRIX VPCubeMapMatrix;
	VPCubeMapMatrix = cubeFaceVP;


	pShaders->BindVSandIA(ShaderPicker::Light_VS_PS);
	pShaders->BindPS(ShaderPicker::Light_VS_PS);
	DirectX::XMMATRIX skullOrbiting;
	DirectX::XMMATRIX axisRotation;
	axisRotation = DirectX::XMMatrixRotationY(timer.TotalTime());

	skullOrbiting = axisRotation * shapes.Get_m_CenterSphere() * shapes.GetCameraOffset() * DirectX::XMMatrixTranslation(25.0f, 0.0f, 0.0f)
		* DirectX::XMMatrixRotationY(timer.TotalTime());
	pSkull->UpdateVSMatrices(wnd.GetGraphics(), skullOrbiting * DirectX::XMMatrixScaling(0.1f, 0.1f, 0.1f), VPCubeMapMatrix);
	pSkull->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
	pSkull->BindAndDrawIndexed(wnd.GetGraphics());


	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::LightAndTexture_VS_PS);

	pBox->UpdateVSMatrices(wnd.GetGraphics(), shapes.Get_m_BoxWorld() * shapes.GetCameraOffset(), VPCubeMapMatrix);
	pBox->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
	pBox->BindAndDrawIndexed(wnd.GetGraphics());


	pHills->UpdateVSMatrices(wnd.GetGraphics(), shapes.Get_m_GridWorld() * shapes.GetCameraOffset(), VPCubeMapMatrix, camera.GetCameraPosition());
	pHills->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
	pHills->BindAndDrawIndexed(wnd.GetGraphics());

	for (auto& x : cylinders)
	{
		x->UpdateVSMatrices(wnd.GetGraphics(), *(shapes.GetCylinderWorldArray())++ * shapes.GetCameraOffset(), VPCubeMapMatrix);
		x->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
		x->BindAndDrawIndexed(wnd.GetGraphics());

	}
	shapes.GetCylinderWorldArray() -= 10; //reset array position

	for (auto& x : geoSpheres)
	{
		x->UpdateVSMatrices(wnd.GetGraphics(),
			*(shapes.GetSphereWorldArray())++ * shapes.GetCameraOffset(), VPCubeMapMatrix, sin(timer.TotalTime()));
		x->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
		x->BindAndDrawIndexed(wnd.GetGraphics());
	}
	shapes.GetSphereWorldArray() -= 10; //reset array position


	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::NoCullRS);
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(RenderStates::LessEqualDSS, 0u);
	pShaders->BindVSandIA(ShaderPicker::Sky_VS_PS);
	pShaders->BindPS(ShaderPicker::Sky_VS_PS);

	pSky->UpdateVSMatricesAndCubeMap(wnd.GetGraphics(), VPCubeMapMatrix);
	pSky->BindAndDrawIndexed(wnd.GetGraphics());

	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(0u);
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(0u, 0u);

}

void App::CreateShadowMapDemo()
{
	pSky = new Sky(wnd.GetGraphics());
	pDisplacementMappingBox = new Box(wnd.GetGraphics(), 1.5f, 1.5f, 2.5f, DemoSwitch::DisplacementMapping);
	pSkull = new Skull(wnd.GetGraphics(), L"models\\skull.txt", DemoSwitch::Shapesdemo);
	pHills = new Hills(wnd.GetGraphics(), 25.0f, 25.0f, 65, 45, DemoSwitch::ShadowMap);
	for (int i = 0; i < 10; i++)
	{
		cylinders.push_back(new Cylinder(wnd.GetGraphics(), 0.5f, 0.3f, 3.0f, 20, 20, DemoSwitch::Shapesdemo));
	}
	for (int i = 0; i < 10; i++)
	{
		displacementCylinders.push_back(new Cylinder(wnd.GetGraphics(), 0.5f, 0.3f, 3.0f, 20, 20, DemoSwitch::DisplacementMapping));
	}
	for (size_t i = 0; i < 10; i++)
	{
		geoSpheres.push_back(new GeoSphere(wnd.GetGraphics(), 0.5f, 2u, false));
	}
}

void App::DrawShadowMapDemo()
{
	const float color[] = { 0.69f, 0.77f, 0.87f, 1.0f };
	wnd.GetGraphics().pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	viewProjectionMatrix = GetViewProjectionCamera();





	pShaders->BindVSandIA(ShaderPicker::Light_VS_PS);
	pShaders->BindPS(ShaderPicker::Light_VS_PS);
	DirectX::XMMATRIX skullOrbiting;
	DirectX::XMMATRIX axisRotation;
	axisRotation = DirectX::XMMatrixRotationY(timer.TotalTime());

	pSkull->UpdateVSMatrices(wnd.GetGraphics(),
		shapes.GetCameraOffset() * DirectX::XMMatrixScaling(0.5f, 0.5f, 0.5f),
		viewProjectionMatrix);
	pSkull->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
	pSkull->BindAndDrawIndexed(wnd.GetGraphics());


	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::LightAndTexture_VS_PS);

// 	if (GetAsyncKeyState('7') & 0x8000)
// 	{
// 		wnd.GetGraphics().pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
// 		pShaders->BindVSandIA(ShaderPicker::DisplacementMapping_VS_DS_HS);
// 		pShaders->BindHS(ShaderPicker::DisplacementMapping_VS_DS_HS);
// 		pShaders->BindDS(ShaderPicker::DisplacementMapping_VS_DS_HS);
// 		pShaders->BindPS(ShaderPicker::DisplacementMapping_VS_DS_HS);
// 	}
// 	if (GetAsyncKeyState('6') & 0x8000)
// 	{
// 		wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::WireframeRS);
// 	}





	for (auto& x : displacementCylinders)
	{
		x->UpdateDisplacementCBuffers(wnd.GetGraphics(), *(shapes.GetCylinderWorldArray())++ * shapes.GetCameraOffset(),
			viewProjectionMatrix, camera.GetCameraPosition());
		x->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
		x->BindAndDrawIndexed(wnd.GetGraphics());

	}
	shapes.GetCylinderWorldArray() -= 10; //reset array position


	pShaders->BindVSandIA(ShaderPicker::ShadowMap_VS_PS);
	pShaders->BindPS(ShaderPicker::ShadowMap_VS_PS);
	pHills->UpdateVSMatrices(wnd.GetGraphics(), shapes.Get_m_GridWorld() * shapes.GetCameraOffset(),
		viewProjectionMatrix, camera.GetCameraPosition());
	pHills->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
	pHills->BindAndDrawIndexed(wnd.GetGraphics());

	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::LightAndTexture_VS_PS);

	pDisplacementMappingBox->UpdateDisplacementCBuffers(wnd.GetGraphics(),
		shapes.Get_m_BoxWorld() * shapes.GetCameraOffset(), viewProjectionMatrix, camera.GetCameraPosition());
	pDisplacementMappingBox->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
	pDisplacementMappingBox->BindAndDrawIndexed(wnd.GetGraphics());
	pShaders->UnbindDS();
	pShaders->UnbindHS();



	wnd.GetGraphics().pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::LightAndTexture_VS_PS);

	for (auto& x : geoSpheres)
	{
		x->UpdateVSMatrices(wnd.GetGraphics(),
			*(shapes.GetSphereWorldArray())++ * shapes.GetCameraOffset(), viewProjectionMatrix, sin(timer.TotalTime()));
		x->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
		x->BindAndDrawIndexed(wnd.GetGraphics());
	}
	shapes.GetSphereWorldArray() -= 10; //reset array position


	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::NoCullRS);
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(RenderStates::LessEqualDSS, 0u);
	pShaders->BindVSandIA(ShaderPicker::Sky_VS_PS);
	pShaders->BindPS(ShaderPicker::Sky_VS_PS);

	pSky->UpdateVSMatricesAndCubeMap(wnd.GetGraphics(), viewProjectionMatrix);
	pSky->BindAndDrawIndexed(wnd.GetGraphics());

	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(0u);
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(0u, 0u);

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

void App::CreatePicking()
{
	pPicking = new Picking(wnd.GetGraphics());
}

void App::DrawPicking()
{
	viewProjectionMatrix = GetViewProjectionCamera();
	pShaders->BindVSandIA(ShaderPicker::Light_VS_PS);
	pShaders->BindPS(ShaderPicker::Light_VS_PS);

	pPicking->UpdateVSMatrices(wnd.GetGraphics(), DirectX::XMMatrixIdentity(), viewProjectionMatrix);
	pPicking->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPosition());
	pPicking->BindAndDrawIndexed(wnd.GetGraphics());
// 	if (GetAsyncKeyState(VK_RBUTTON))
// 	{
// 		pPicking->Pick(camera.GetViewMatrix(), camera.GetProjecion(), wnd.mouse.GetPosX(), wnd.mouse.GetPosY());
// 		if (pPicking->GetPickedTriangleIndex() != -1)
// 		{
			// Change depth test from < to <= so that if we draw the same triangle twice, it will still pass
			// the depth test.  This is because we redraw the picked triangle with a different material
			// to highlight it. 
			wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(RenderStates::LessEqualDSS, 0u);
			pPicking->SetPickedMaterial(wnd.GetGraphics());
			pPicking->BindAndDrawIndexed(wnd.GetGraphics(), 3, 3 * 25, 0u);

// 		}

// 	}



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
