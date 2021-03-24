
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "App.h"
App::App()
	: wnd("Output Window", resolution_width, resolution_height)
{
	rStates.InitializeAll(wnd.GetGraphics());
	pShaders = new Shaders(wnd.GetGraphics());

	pCircle = new Circle(wnd.GetGraphics());



// 	CreateBox();
// 	ShapesDemoCreateShapes();
// 	CreateHillsWithWaves();
// 	MirrorDemoCreate();
// 	LightningCreate();
// 	DepthComplexityStencilCreate();



//  	wnd.GetGraphics().SetProjection(GetPerspectiveProjection());
}

void App::DoFrame()
{

// 	const float c = abs((sin(timer.TotalTime())));
	timer.Tick();
// 	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(RenderStates::NoRenderTargetWritesBS, blendFactorsZero, 0xffffffff);
// 
// 	ShapesDemoDrawShapes();
// 	MirrorDemoDraw();
// 	DrawHillsWithWaves();
// 	DrawBox();
// 	LightningDraw();
// 	DepthComplexityStencilDraw();
// 	pShaders->UnbindGS(); //call it first, so RenderDoc can capture GS


	pShaders->BindVSandIA(ShaderPicker::CircleToCylinderVS_GS_PS);
// 	pShaders->BindGS(ShaderPicker::CircleToCylinderVS_GS_PS);
	pShaders->BindPS(ShaderPicker::CircleToCylinderVS_GS_PS);
// 	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
// 	DirectX::XMMATRIX view = DirectX::XMMatrixIdentity();

	
// 	SetObjectMatrix(DirectX::XMMatrixIdentity());

	//model/world
	DirectX::XMMATRIX model = DirectX::XMMatrixIdentity();
	//model = DirectX::XMMatrixRotationZ((timer.TotalTime())) * DirectX::XMMatrixTranslation(0.5f, -0.5f, 0.0f) ;
	//reverse reading order
	model = DirectX::XMMatrixRotationZ((timer.TotalTime())) /** DirectX::XMMatrixTranslation(-1.5f, 1.5f, 0.0f) */;


	//projection
	DirectX::XMMATRIX projection = GetPerspectiveProjection();

	//learnopengl camera
	GLCamera = GetCamera();

	//reversed order from opengl
	DirectX::XMMATRIX clipMatrix = model * GLCamera * projection ;


	pCircle->UpdateVSMatrices(wnd.GetGraphics(), clipMatrix);
// 	pCircle->UpdateVertexConstantBuffer(wnd.GetGraphics());
	pCircle->BindAndDrawIndexed(wnd.GetGraphics());


	model = DirectX::XMMatrixRotationZ((-timer.TotalTime())) * DirectX::XMMatrixTranslation(1.5f, 1.5f, 0.0f);
	clipMatrix = model * GLCamera * projection;

	pCircle->UpdateVSMatrices(wnd.GetGraphics(), clipMatrix);
	pCircle->BindAndDrawIndexed(wnd.GetGraphics());


	ScrollWheelCounter();
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
	oss << "X: " << wnd.mouse.GetPosX() << " Y: " << wnd.mouse.GetPosY() << " Yaw: " << GetYaw() <<
		 " Pitch: " << GetPitch();
	wnd.SetTitle(oss.str().c_str());
}

void App::ScrollWheelCounter()
{
	while (!wnd.mouse.IsEmpty())
	{
		const Mouse::Event e = wnd.mouse.Read();
		switch (e.GetType())
		{
			case Mouse::Event::Type::MWheelUp:
			{
				zoom += 0.1f;
			}
			break;

			case Mouse::Event::Type::MWheelDown:
			{
				zoom -= 0.1f;
			}
			break;
		}
	}
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



void App::CameraMove()
{


}



void App::TwoTestCubes() noexcept
{
	wnd.GetGraphics().TestDrawing(timer.TotalTime(), 0.0, 0.0f, 4.0f);
// 	wnd.GetGraphics().TestDrawing((timer.TotalTime() * 0.5f), axis_x, axis_y, axis_z);
}




void App::DrawHillsWithWaves()
{
 	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(RenderStates::srsColor, blendFactorsZero, 0xffffffff);
	pShaders->UnbindGS(); //call it first, so RenderDoc can capture GS

	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::LightAndTexture_VS_PS);

	pHills->SetCameraMatrix(mCamera * CameraZoom());
	pHills->Update(timer.TotalTime());
	pHills->UpdateConstantBuffers(wnd.GetGraphics(),  wEyePosition, pos, target); //offsetForHillsWithWaves
	pHills->BindAndDrawIndexed(wnd.GetGraphics());
	SetObjectMatrix(offsetForHillsWithWaves);

	//transparency for the box achieved with clip in PS, so this isn't necessary?
// 	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(RenderStates::TransparentBS, blendFactorsZero, 0xffffffff);

	pWaves->SetCameraMatrix(mCamera * CameraZoom());
	pWaves->BindAndDrawIndexed(wnd.GetGraphics());
	pWaves->UpdateScene(timer.TotalTime(), timer.DeltaTime(), wnd.GetGraphics(), wEyePosition);
	pWaves->UpdateVertexConstantBuffer(wnd.GetGraphics());
	SetObjectMatrix(DirectX::XMMatrixIdentity());

	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::NoCullRS);
	SetObjectMatrix(DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f));
	pBox->SetCameraMatrix(mCamera * CameraZoom());
	pBox->Update(timer.TotalTime());
	pBox->UpdateVertexConstantBuffer(wnd.GetGraphics());
	pBox->BindAndDrawIndexed(wnd.GetGraphics());


	pShaders->BindVSandIA(ShaderPicker::TreeBillboardVS_PS_GS);
	pShaders->BindGS(ShaderPicker::TreeBillboardVS_PS_GS);
	pShaders->BindPS(ShaderPicker::TreeBillboardVS_PS_GS);
	pBillboards->SetCameraMatrix(mCamera * CameraZoom());
	pBillboards->Update(timer.TotalTime());
	pBillboards->UpdateConstantBuffers(wnd.GetGraphics(), wEyePosition);
	pBillboards->BindAndDraw(wnd.GetGraphics(), 25u, 0u);


// 	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(0u); reset isn't necessary?

}

void App::CreateHillsWithWaves()
{
 	pHills = new Hills(wnd.GetGraphics(), 160.0f, 160.0f, 50u, 50u, DemoSwitch::HillsDemo);
	pWaves = new WaveSurface(wnd.GetGraphics());
	pBox = new Box(wnd.GetGraphics(), 5.0f, 5.0f, 5.0f, DemoSwitch::DefaultBox);
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
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::NoCullRS);

	SetObjectMatrix(DirectX::XMMatrixTranslation(0.0f, -5.0f, 0.0f));
	pBox->SetCameraMatrix(mCamera * CameraZoom());
	pBox->UpdateVertexConstantBuffer(wnd.GetGraphics());
	pBox->BindAndDrawIndexed(wnd.GetGraphics());
}

void App::MirrorDemoCreate()
{
	pBox = new Box(wnd.GetGraphics(), 2.5f, 2.5f, 1.5f, DemoSwitch::Shapesdemo);
	pSkull = new Skull(wnd.GetGraphics(), L"models\\skull.txt");
	pMirrorRoom = new MirrorRoom(wnd.GetGraphics());
}

void App::MirrorDemoDraw()
{
	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::MirrorRoomPS);

	SetObjectMatrix(DirectX::XMMatrixIdentity());

	pMirrorRoom->SetCameraMatrix(mCamera * CameraZoom());

	//floor
	pMirrorRoom->UpdateMirrorRoomConstBuffers(wnd.GetGraphics(), 2u);
	pMirrorRoom->BindAndDraw(wnd.GetGraphics(), 6u, 0u);


	pMirrorRoom->UpdateMirrorRoomConstBuffers(wnd.GetGraphics(), 0u);
	pMirrorRoom->BindAndDraw(wnd.GetGraphics(), 18u, 6u);

	//rotate reflection and the original
//  	mirroredSkull = DirectX::XMMatrixRotationY(abs((sin(timer.DeltaTime())))) * mirroredSkull;
	SetObjectMatrix(mirroredSkull);
	pShaders->BindVSandIA(ShaderPicker::Light_VS_PS);
	pShaders->BindPS(ShaderPicker::Light_VS_PS);
	pSkull->SetCameraMatrix( DirectX::XMMatrixScaling(0.3f, 0.3f, 0.3f) * mCamera * CameraZoom());
	pSkull->UpdateVertexConstantBuffer(wnd.GetGraphics());
	pSkull->BindAndDrawIndexed(wnd.GetGraphics());
	
	// Do not write to render target
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(RenderStates::NoRenderTargetWritesBS, blendFactorsZero, 0xffffffff);
	// Render visible mirror pixels to stencil buffer.
	// Do not write mirror depth to depth buffer at this point, otherwise it will occlude the reflection.
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(RenderStates::MarkMirrorDSS, 1);
	// draw mirror
	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::MirrorRoomPS);
	pMirrorRoom->UpdateMirrorRoomConstBuffers(wnd.GetGraphics(), 1u);
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
	DirectX::XMFLOAT3 lightDirection;
	ZeroMemory(&lightDirection, sizeof(DirectX::XMFLOAT3));
	DirectX::XMVECTOR lightDir;
	ZeroMemory(&lightDir, sizeof(DirectX::XMVECTOR));
	DirectX::XMVECTOR reflectedLD;
	ZeroMemory(&reflectedLD, sizeof(DirectX::XMVECTOR));
	UINT numOfLights{ 3 };
	for (UINT i = 0; i < numOfLights; i++)
	{
		oldLIghtDirection[i] = pSkull->GetLight(i).direction;
		lightDirection = pSkull->GetLight(i).direction;
		lightDir.m128_f32[0] = lightDirection.x;
		lightDir.m128_f32[1] = lightDirection.y;
		lightDir.m128_f32[2] = lightDirection.z;
		reflectedLD = DirectX::XMVector3TransformNormal(lightDir, R);
		DirectX::XMStoreFloat3(&reflectedLightDirection, reflectedLD);
		pSkull->SetNewLightDirection(reflectedLightDirection, i);
	}
	pSkull->UpdateLightDirection(wnd.GetGraphics());
	pSkull->UpdateEyePosition(wEyePosition);


	//cull clockwise triangles for reflections.
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::CullClockwiseRS);

	// Only draw reflection into visible mirror pixels as marked by the stencil buffer. 
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(RenderStates::DrawReflectionDSS, 1);
	//draw reflected skull
	pShaders->BindVSandIA(ShaderPicker::Light_VS_PS);
	pShaders->BindPS(ShaderPicker::Light_VS_PS);
	SetObjectMatrix(mirroredSkull * R);
	pSkull->SetCameraMatrix(DirectX::XMMatrixScaling(0.3f, 0.3f, 0.3f) * mCamera * CameraZoom());
	pSkull->UpdateVertexConstantBuffer(wnd.GetGraphics());
	pSkull->BindAndDrawIndexed(wnd.GetGraphics());
	// Restore default states.
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(0);
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(0, 0);

	//restore light direction
	for (UINT i = 0; i < numOfLights; ++i)
	{
		pSkull->SetNewLightDirection(oldLIghtDirection[i], i);
	}
	pSkull->UpdateLightDirection(wnd.GetGraphics());

	// Draw the mirror to the back buffer as usual but with transparency
	// blending so the reflection shows through.
	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::MirrorRoomPS);
 	pMirrorRoom->UpdateMirrorRoomConstBuffers(wnd.GetGraphics(), 1u);
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(RenderStates::TransparentBS, blendFactorsZero, 0xffffffff);

	pMirrorRoom->BindAndDraw(wnd.GetGraphics(), 6u, 24u);


	//shadow
	DirectX::XMVECTOR shadowPlane = DirectX::XMVectorSet(0.0, 1.0f, 0.0, 0.0f); //xz plane
	DirectX::XMFLOAT3 toMainLightTemp = pSkull->GetLight(0).direction;
	using namespace DirectX; //for - sign
	DirectX::XMVECTOR toMainLight = -DirectX::XMLoadFloat3(&toMainLightTemp);
	DirectX::XMMATRIX S = DirectX::XMMatrixShadow(shadowPlane, toMainLight);
	DirectX::XMMATRIX shadowOffsetY = DirectX::XMMatrixTranslation(0.0f, 0.001f, 0.0f);
	pSkull->UpdateMaterial(wnd.GetGraphics(), true);

	SetObjectMatrix(mirroredSkull * S * shadowOffsetY);
	pSkull->SetCameraMatrix(DirectX::XMMatrixScaling(0.3f, 0.3f, 0.3f) * mCamera * CameraZoom());
	pSkull->UpdateVertexConstantBuffer(wnd.GetGraphics());
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(RenderStates::NoDoubleBlendDSS, 0);
	pShaders->BindVSandIA(ShaderPicker::Light_VS_PS);
	pShaders->BindPS(ShaderPicker::Light_VS_PS);
	pSkull->BindAndDrawIndexed(wnd.GetGraphics());
	// Restore default states.
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(0);
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(0, 0);

	//restore the material state
	pSkull->UpdateLightDirection(wnd.GetGraphics());


}

void App::LightningCreate()
{
	pCylinder = new Cylinder(wnd.GetGraphics(), 3.0f, 3.0f, 5.0f, 15u, 15u, DemoSwitch::LightningCone);
}

void App::LightningDraw()
{
// 	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(RenderStates::srsColor, blendFactorsZero, 0xffffffff);

// 	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(RenderStates::TransparentBS, blendFactorsZero, 0xffffffff);
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::NoCullRS);

	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::LightAndTextureArrayPS);

	SetObjectMatrix(DirectX::XMMatrixIdentity());
	pCylinder->SetCameraMatrix(mCamera * CameraZoom());
	pCylinder->Update(timer.TotalTime());
	pCylinder->UpdateVertexConstantBuffer(wnd.GetGraphics());
	
		pCylinder->IncrementTexArrPos();

	pCylinder->BindAndDrawIndexed(wnd.GetGraphics());

// 	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(0u);// reset isn't necessary?

}

void App::DepthComplexityStencilDraw()
{
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(RenderStates::DepthComplexityCountDSS, 0);

	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::LightAndTexture_VS_PS);

	pHills->SetCameraMatrix(mCamera * CameraZoom());
	pHills->Update(timer.TotalTime());
	pHills->UpdateConstantBuffers(wnd.GetGraphics(), wEyePosition, pos, target); //offsetForHillsWithWaves
	pHills->BindAndDrawIndexed(wnd.GetGraphics());
	SetObjectMatrix(offsetForHillsWithWaves);


	pWaves->SetCameraMatrix(mCamera * CameraZoom());
	pWaves->BindAndDrawIndexed(wnd.GetGraphics());
	pWaves->UpdateScene(timer.TotalTime(), timer.DeltaTime(), wnd.GetGraphics(), wEyePosition);
	pWaves->UpdateVertexConstantBuffer(wnd.GetGraphics());
	SetObjectMatrix(DirectX::XMMatrixIdentity());

	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::NoCullRS);
	SetObjectMatrix(DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f));
	pBox->SetCameraMatrix(mCamera * CameraZoom());
	pBox->Update(timer.TotalTime());
	pBox->UpdateVertexConstantBuffer(wnd.GetGraphics());
	pBox->BindAndDrawIndexed(wnd.GetGraphics());

	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::NoCullRS);
	SetObjectMatrix(DirectX::XMMatrixTranslation(0.0f, -5.0f, 0.0f));
	pShaders->BindVSandIA(ShaderPicker::DepthComplexityVS_PS);
	pShaders->BindPS(ShaderPicker::DepthComplexityVS_PS);

	pDepthArr[0]->SetCameraMatrix(mCamera /** CameraZoom()*/);
	pDepthArr[0]->UpdateVertexConstantBuffer(wnd.GetGraphics());
	pDepthArr[1]->SetCameraMatrix(mCamera /** CameraZoom()*/);
	pDepthArr[1]->UpdateVertexConstantBuffer(wnd.GetGraphics());
	pDepthArr[2]->SetCameraMatrix(mCamera /** CameraZoom()*/);
	pDepthArr[2]->UpdateVertexConstantBuffer(wnd.GetGraphics());

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

	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(0, 0);

}

void App::DepthComplexityStencilCreate()
{
	pDepthArr[0] = new DepthComplexity(wnd.GetGraphics());
	pDepthArr[1] = new DepthComplexity(wnd.GetGraphics());
	pDepthArr[2] = new DepthComplexity(wnd.GetGraphics());
	pHills = new Hills(wnd.GetGraphics(), 160.0f, 160.0f, 50u, 50u, DemoSwitch::HillsDemo);
	pWaves = new WaveSurface(wnd.GetGraphics());
	pBox = new Box(wnd.GetGraphics(), 5.0f, 5.0f, 5.0f, DemoSwitch::DefaultBox);

}

DirectX::XMMATRIX App::CameraZoom() const noexcept
{
	return DirectX::XMMatrixTranslation(0.0f, -zoom, zoom);
}

DirectX::XMMATRIX App::GetPerspectiveProjection() noexcept
{

	return   DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PI * 0.25f, screenAspect, 1.0f, 1000.0f);

// 	return DirectX::XMMatrixOrthographicLH(resolution_width, resolution_height, 0.1f, 100.0f);

}





void App::ShapesDemoCreateShapes()
{
	pBox = new Box(wnd.GetGraphics(), 1.5f, 1.5f, 2.5f, DemoSwitch::Shapesdemo);
 	//pGeoSphere = new GeoSphere(wnd.GetGraphics(), 0.5f, 20u);
 	pSkull = new Skull(wnd.GetGraphics(), L"models\\skull.txt");
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

void App::ShapesDemoDrawShapes()
{
	pShaders->BindVSandIA(ShaderPicker::Light_VS_PS);
	pShaders->BindPS(ShaderPicker::Light_VS_PS);

	SetObjectMatrix(shapes.Get_m_CenterSphere() * shapes.GetCameraOffset());
	pSkull->SetCameraMatrix(DirectX::XMMatrixScaling(0.3f, 0.3f, 0.3f) * mCamera * CameraZoom());
	pSkull->UpdateVertexConstantBuffer(wnd.GetGraphics());
	pSkull->BindAndDrawIndexed(wnd.GetGraphics());

	pShaders->BindVSandIA(ShaderPicker::LightAndTexture_VS_PS);
	pShaders->BindPS(ShaderPicker::LightAndTexture_VS_PS);


	SetObjectMatrix(shapes.Get_m_BoxWorld() * shapes.GetCameraOffset());
	pBox->UpdateVertexConstantBuffer(wnd.GetGraphics());
	pBox->SetCameraMatrix(mCamera * CameraZoom());
	pBox->BindAndDrawIndexed(wnd.GetGraphics());


	SetObjectMatrix(shapes.Get_m_GridWorld() * shapes.GetCameraOffset());
	pHills->SetCameraMatrix(mCamera * DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f) * CameraZoom());
	pHills->Update(timer.TotalTime());
	pHills->UpdateConstantBuffers(wnd.GetGraphics(), wEyePosition, pos, target);
	pHills->BindAndDrawIndexed(wnd.GetGraphics());

	for (auto& x : cylinders)
	{
		SetObjectMatrix(*(shapes.GetCylinderWorldArray())++ * shapes.GetCameraOffset());
		x->SetCameraMatrix(mCamera * CameraZoom());
		x->UpdateVertexConstantBuffer(wnd.GetGraphics());
		x->BindAndDrawIndexed(wnd.GetGraphics());
 	
 	}
 	shapes.GetCylinderWorldArray() -= 10; //reset array position

	for (auto& x : geoSpheres)
	{
		SetObjectMatrix(*(shapes.GetSphereWorldArray())++ * shapes.GetCameraOffset());
		x->SetCameraMatrix(mCamera * CameraZoom());
		x->UpdateVertexConstantBuffer(wnd.GetGraphics());
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
DirectX::XMMATRIX App::GetCamera() noexcept
{
	//for vector arithmetics
	using namespace DirectX;

	DirectX::XMVECTOR cameraPos = { 0.0f, 0.0f, -6.0f };
	DirectX::XMVECTOR cameraTarget = { 0.0f, 0.0f, 0.0f };
	DirectX::XMVECTOR cameraReverseDirection = DirectX::XMVector4Normalize(cameraPos - cameraTarget);

	DirectX::XMVECTOR up = { 0.0f, 1.0f, 0.0f };
	DirectX::XMVECTOR cameraRight = DirectX::XMVector4Normalize(DirectX::XMVector3Cross(up, cameraReverseDirection));

	DirectX::XMVECTOR cameraUp = DirectX::XMVector4Normalize(DirectX::XMVector3Cross(cameraReverseDirection, cameraRight));

	//rotational cam
	const float radius = 10.0f;
	float camX = sin(timer.TotalTime()) * radius;
	float camZ = cos(timer.TotalTime()) * radius;
	DirectX::FXMVECTOR camRotatePos{ camX, 0.0f, camZ };
	DirectX::FXMMATRIX viewAround = DirectX::XMMatrixLookAtLH(camRotatePos, cameraTarget, cameraUp);
	//default cam
	DirectX::FXMMATRIX view = DirectX::XMMatrixLookAtLH(cameraPos, cameraTarget, cameraUp);

	//movement cam
	const float cameraSpeed = 2.5f * timer.DeltaTime();
	if (GetAsyncKeyState('W') & 0x8000)
	{
		camPos += cameraSpeed * camFront;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		camPos -= cameraSpeed * camFront;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		camPos -= DirectX::XMVector4Normalize(DirectX::XMVector3Cross(camFront, camUp)) * cameraSpeed;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		camPos += DirectX::XMVector4Normalize(DirectX::XMVector3Cross(camFront, camUp)) * cameraSpeed;
	}
	DirectX::FXMMATRIX viewMov = 
		DirectX::XMMatrixLookAtLH(camPos, camPos + GetCameraFront(wnd.mouse.GetPosX(), wnd.mouse.GetPosY()), camUp);
	//GetCameraFront(wnd.mouse.GetPosX(), wnd.mouse.GetPosY())

	return viewMov;
}

DirectX::XMVECTOR App::GetCameraFront(int xPos, int yPos) noexcept
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xOffset = (float)xPos - (float)lastX;
	float yOffset = (float)yPos - (float)lastY;
	lastX = xPos;
	lastY = yPos;

	const float sensitivity = 0.01f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	//reset to original position
	if (wnd.mouse.IsLeftPressed())
	{
		yaw = 45.5f;
		pitch = 0.0f;
	}
	//is this multiplication necessary?
	DirectX::XMVECTOR direction{ -cos(yaw) * cos(pitch), -sin(pitch), sin(yaw) * cos(pitch) };
	camFront = DirectX::XMVector3Normalize(direction);
	return camFront;
}

float App::GetYaw()
{
	return yaw;
}

float App::GetPitch()
{
	return pitch;
}

App::~App()
{
}
