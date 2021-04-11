
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "App.h"
App::App()
	: wnd("Output Window", resolution_width, resolution_height)
{
	rStates.InitializeAll(wnd.GetGraphics());
	pShaders = new Shaders(wnd.GetGraphics());




// 	CreateBox();
// 	CreateShapes();
	CreateHillsWithWaves();
// 	CreateMirror();
// 	CreateLightning();
// 	CreateDepthComplexityStencil();



//  	wnd.GetGraphics().SetProjection(GetPerspectiveProjection());
}

void App::DoFrame()
{

// 	const float c = abs((sin(timer.TotalTime())));
	timer.Tick();
// 	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(RenderStates::NoRenderTargetWritesBS, blendFactorsZero, 0xffffffff);
// 
// 	DrawShapes();
// 	DrawMirror();
	DrawHillsWithWaves();
// 	DrawBox();
// 	DrawLightning();
// 	DrawDepthComplexityStencil();


	

	CalculateFrameStats();

	DebugTextToTitle();
	wnd.GetGraphics().EndFrame();
	wnd.GetGraphics().ClearBuffer(0.69f, 0.77f, 0.87f);

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
				<< "; Frame Time: " << ms_per_frame << " ms., TotalTime: " << static_cast<UINT>(timer.TotalTime()) << " s. ";
			wnd.SetTitle(oss.str().c_str());
		frameCount = 0;
		timeElapsed += 1.0f;
	}
}


void App::DrawHillsWithWaves()
{
 	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(RenderStates::srsColor, blendFactorsZero, 0xffffffff);
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::CullCounterClockwiseRS);

	pShaders->UnbindGS(); //call it first, so RenderDoc can capture GS

	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::HillsAllLight_PS);

	viewProjectionMatrix = GetViewProjectionCamera();
	pHills->UpdateVSMatrices(wnd.GetGraphics(), pHills->GetHillsOffset(), viewProjectionMatrix);
	//pHills->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPositionFloat());
	pHills->UpdatePSAllLights(wnd.GetGraphics(), camera.GetCameraPositionFloat(), camera.GetCameraDirection(), timer.TotalTime());
	pHills->BindAndDrawIndexed(wnd.GetGraphics());

	pShaders->BindPS(ShaderPicker::LightAndTexture_VS_PS);
	//transparency for the box achieved with clip in PS, so this isn't necessary?
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(RenderStates::TransparentBS, blendFactorsZero, 0xffffffff);
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::NoCullRS);
	pBox->UpdateVSMatrices(wnd.GetGraphics(), pBox->GetBoxForHillsOffset(), viewProjectionMatrix);
	pBox->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPositionFloat());;
	pBox->BindAndDrawIndexed(wnd.GetGraphics());

	pWaves->BindAndDrawIndexed(wnd.GetGraphics());
	pWaves->UpdateScene(timer.TotalTime(), timer.DeltaTime(), wnd.GetGraphics());
	pWaves->UpdateVSMatrices(wnd.GetGraphics(), pWaves->GetWaveSurfaceOffset(), viewProjectionMatrix);
	pWaves->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPositionFloat());

	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(RenderStates::srsColor, blendFactorsZero, 0xffffffff);

	pShaders->BindVSandIA(ShaderPicker::TreeBillboardVS_PS_GS);
	pShaders->BindGS(ShaderPicker::TreeBillboardVS_PS_GS);
	pShaders->BindPS(ShaderPicker::TreeBillboardVS_PS_GS);
	pBillboards->UpdateConstantBuffers(wnd.GetGraphics(), viewProjectionMatrix, camera.GetCameraPositionFloat());
	pBillboards->BindAndDraw(wnd.GetGraphics(), 25u, 0u);



}

void App::CreateHillsWithWaves()
{
 	pHills = new Hills(wnd.GetGraphics(), 160.0f, 160.0f, 50u, 50u, DemoSwitch::HillsAllLight);
	pWaves = new WaveSurface(wnd.GetGraphics());
	pBox = new Box(wnd.GetGraphics(), 5.0f, 5.0f, 5.0f, DemoSwitch::HillsDemo);
	pBillboards = new TreeBillboard(wnd.GetGraphics());
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

	pBox->UpdateVSMatrices(wnd.GetGraphics(), DirectX::XMMatrixIdentity(), viewProjectionMatrix);
	pBox->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPositionFloat());
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
	pMirrorRoom->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPositionFloat());
	pMirrorRoom->SwitchTexture(wnd.GetGraphics(), 2u);
	pMirrorRoom->BindAndDraw(wnd.GetGraphics(), 6u, 0u);
	//wall
	pMirrorRoom->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPositionFloat());
	pMirrorRoom->SwitchTexture(wnd.GetGraphics(), 0u);
	pMirrorRoom->BindAndDraw(wnd.GetGraphics(), 18u, 6u);

	//rotate reflection and the original
//  	mirroredSkull = DirectX::XMMatrixRotationY(abs((sin(timer.DeltaTime())))) * mirroredSkull;
	pShaders->BindVSandIA(ShaderPicker::Light_VS_PS);
	pShaders->BindPS(ShaderPicker::MirrorSkull_PS);
	pSkull->UpdateVSMatrices(wnd.GetGraphics(), pSkull->GetMirroredSkullTranslation() * DirectX::XMMatrixScaling(0.3f, 0.3f, 0.3f), viewProjectionMatrix);
	pSkull->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPositionFloat());
	pSkull->BindAndDrawIndexed(wnd.GetGraphics());
	
	// Do not write to render target
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(RenderStates::NoRenderTargetWritesBS, blendFactorsZero, 0xffffffff);
	// Render visible mirror pixels to stencil buffer.
	// Do not write mirror depth to depth buffer at this point, otherwise it will occlude the reflection.
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(RenderStates::MarkMirrorDSS, 1);
	// draw mirror
	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::MirrorRoomPS);
	pMirrorRoom->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPositionFloat());
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
	pMirrorRoom->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPositionFloat());
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
	pMirrorRoom->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPositionFloat());
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

	pSkull->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPositionFloat());
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(RenderStates::NoDoubleBlendDSS, 0);
	pSkull->BindAndDrawIndexed(wnd.GetGraphics());
	// Restore default states.
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(0);
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(0, 0);



}

void App::CreateLightning()
{
	pCylinder = new Cylinder(wnd.GetGraphics(), 3.0f, 3.0f, 5.0f, 15u, 15u, DemoSwitch::LightningCone);
}

void App::DrawLightning()
{
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::NoCullRS);
	viewProjectionMatrix = GetViewProjectionCamera();
	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::LightAndTextureArrayPS);

	pCylinder->UpdateVSMatrices(wnd.GetGraphics(), DirectX::XMMatrixIdentity(), viewProjectionMatrix);
// 	pCylinder->Update(timer.TotalTime());
	
	pCylinder->IncrementTexArrPos();
	pCylinder->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPositionFloat());

	pCylinder->BindAndDrawIndexed(wnd.GetGraphics());

}

void App::DrawDepthComplexityStencil()
{
	viewProjectionMatrix = GetViewProjectionCamera();
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(RenderStates::DepthComplexityCountDSS, 0);
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::CullCounterClockwiseRS);


	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::LightAndTexture_VS_PS);

	pHills->UpdateVSMatrices(wnd.GetGraphics(), pHills->GetHillsOffset(), viewProjectionMatrix);
	pHills->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPositionFloat());
	pHills->BindAndDrawIndexed(wnd.GetGraphics());

// disable transparency for overdraw counting
// 	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(RenderStates::TransparentBS, blendFactorsZero, 0xffffffff);
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::NoCullRS);
// 
	pBox->UpdateVSMatrices(wnd.GetGraphics(), pBox->GetBoxForHillsOffset(), viewProjectionMatrix);
	pBox->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPositionFloat());
	pBox->BindAndDrawIndexed(wnd.GetGraphics());

	pWaves->BindAndDrawIndexed(wnd.GetGraphics());
	pWaves->UpdateScene(timer.TotalTime(), timer.DeltaTime(), wnd.GetGraphics());
	pWaves->UpdateVSMatrices(wnd.GetGraphics(), pWaves->GetWaveSurfaceOffset(), viewProjectionMatrix);
	pWaves->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPositionFloat());


	pShaders->BindVSandIA(ShaderPicker::DepthComplexityVS_PS);
	pShaders->BindPS(ShaderPicker::DepthComplexityVS_PS);

	pDepthArr[0]->UpdateVSMatrices(wnd.GetGraphics(), DirectX::XMMatrixTranslation(0.0f, 0.0f, 3.0f), viewProjectionMatrix);
	pDepthArr[1]->UpdateVSMatrices(wnd.GetGraphics(), DirectX::XMMatrixTranslation(0.0f, 0.0f, 3.0f), viewProjectionMatrix);
	pDepthArr[2]->UpdateVSMatrices(wnd.GetGraphics(), DirectX::XMMatrixTranslation(0.0f, 0.0f, 3.0f), viewProjectionMatrix);

	////////////////
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
	pWaves = new WaveSurface(wnd.GetGraphics());
	pBox = new Box(wnd.GetGraphics(), 5.0f, 5.0f, 5.0f, DemoSwitch::HillsDemo);

}

DirectX::XMMATRIX App::CameraZoom() const noexcept
{
	return DirectX::XMMatrixTranslation(0.0f, -zoom, zoom);
}

DirectX::XMMATRIX App::GetPerspectiveProjection(float in_FOV) noexcept
{
	return   DirectX::XMMatrixPerspectiveFovLH(0.25f * DirectX::XM_PI, screenAspect, 1.0f, 1000.0f);

// 	return DirectX::XMMatrixOrthographicLH(resolution_width, resolution_height, 0.1f, 100.0f);

}





DirectX::XMMATRIX App::GetViewProjectionCamera()
{
	return 	viewProjectionMatrix = camera.GetViewProjection(
		wnd.mouse.GetPosX(),
		wnd.mouse.GetPosY(),
		wnd.mouse.IsLeftPressed(),
		timer.DeltaTime(),
		wnd);
}

void App::CreateShapes()
{
	pBox = new Box(wnd.GetGraphics(), 1.5f, 1.5f, 2.5f, DemoSwitch::Shapesdemo);
 	//pGeoSphere = new GeoSphere(wnd.GetGraphics(), 0.5f, 20u);
 	pSkull = new Skull(wnd.GetGraphics(), L"models\\skull.txt", DemoSwitch::Shapesdemo);
	pHills = new Hills(wnd.GetGraphics(), 25.0f, 25.0f, 65, 45, DemoSwitch::Shapesdemo);
	for (int i = 0; i < 10; i++)
	{
		cylinders.push_back(new Cylinder(wnd.GetGraphics(), 0.5f, 0.3f, 3.0f, 20, 20, DemoSwitch::Shapesdemo));
	}

	for (size_t i = 0; i < 10; i++)
	{
		geoSpheres.push_back(new GeoSphere(wnd.GetGraphics(), 0.5f, 2u));
	}
}

void App::DrawShapes()
{
	viewProjectionMatrix = GetViewProjectionCamera();
	pShaders->BindVSandIA(ShaderPicker::Light_VS_PS);
	pShaders->BindPS(ShaderPicker::Light_VS_PS);

	pSkull->UpdateVSMatrices(wnd.GetGraphics(), shapes.Get_m_CenterSphere() * shapes.GetCameraOffset() * DirectX::XMMatrixScaling(0.3f, 0.3f, 0.3f), viewProjectionMatrix);
	pSkull->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPositionFloat());
	pSkull->BindAndDrawIndexed(wnd.GetGraphics());


	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::LightAndTexture_VS_PS);


	pBox->UpdateVSMatrices(wnd.GetGraphics(), shapes.Get_m_BoxWorld() * shapes.GetCameraOffset(), viewProjectionMatrix);
	pBox->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPositionFloat());
	pBox->BindAndDrawIndexed(wnd.GetGraphics());

	pHills->UpdateVSMatrices(wnd.GetGraphics(), shapes.Get_m_GridWorld() * shapes.GetCameraOffset(), viewProjectionMatrix);
	pHills->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPositionFloat());
	pHills->BindAndDrawIndexed(wnd.GetGraphics());

	for (auto& x : cylinders)
	{
		x->UpdateVSMatrices(wnd.GetGraphics(), *(shapes.GetCylinderWorldArray())++ * shapes.GetCameraOffset(), viewProjectionMatrix);
		x->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPositionFloat());
		x->BindAndDrawIndexed(wnd.GetGraphics());
 	
 	}
 	shapes.GetCylinderWorldArray() -= 10; //reset array position

	for (auto& x : geoSpheres)
	{
		x->UpdateVSMatrices(wnd.GetGraphics(), *(shapes.GetSphereWorldArray())++ * shapes.GetCameraOffset(), viewProjectionMatrix);
		x->UpdatePSConstBuffers(wnd.GetGraphics(), camera.GetCameraPositionFloat());
		x->BindAndDrawIndexed(wnd.GetGraphics());
	}
	shapes.GetSphereWorldArray() -= 10; //reset array position

}

void App::SetObjectMatrix(DirectX::XMMATRIX in_matrix)
{
	// Convert Spherical to Cartesian coordinates.
	float eyePosition = wnd.GetRadius() * sinf(wnd.GetPhi()) * cosf(wnd.GetTheta());
	float focusPosition = wnd.GetRadius() * sinf(wnd.GetPhi()) * sinf(wnd.GetTheta());
	float upDirection = wnd.GetRadius() * cosf(wnd.GetPhi());
	// Build the view matrix.
	wEyePosition = {eyePosition, focusPosition, upDirection};
	pos = DirectX::XMVectorSet(eyePosition, focusPosition, upDirection, 1.0f);

	target = DirectX::XMVectorZero();
	up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	mCamera = DirectX::XMMatrixLookAtLH(pos, target, up);

}


App::~App()
{
}
