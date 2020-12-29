
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "App.h"
App::App()
	: wnd("Output Window", resolution_width, resolution_height)
{
	rStates.InitializeAll(wnd.GetGraphics());

  	CreateBox();
// 	ShapesDemoCreateShapes();
// 	CreateHillsWithWaves();
	MirrorDemoCreate();
 	wnd.GetGraphics().SetProjection(CalculateProjection());
}

void App::DoFrame()
{

// 	const float c = abs((sin(timer.TotalTime())));
	timer.Tick();
// 	ShapesDemoDrawShapes();
	MirrorDemoDraw();
// 	wnd.GetGraphics().pgfx_pDevice->CreateBlendState(&blendDesc1, &pBlendState1);
// 	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(pBlendState1, blendFactors1, 0xffffffff);

// 	DrawHillsWithWaves();
// 	DrawBox();

	ScrollWheelCounter();

	CalculateFrameStats();
	//DebugTextToTitle();



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
	oss << "X: " << wnd.mouse.GetPosX() << " Y: " << wnd.mouse.GetPosY() << " Theta: " << wnd.GetTheta() <<
		 " Radius: " << wnd.GetRadius() << " Phi: " << wnd.GetPhi();
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
				<< "; Frame Time: " << ms_per_frame << " ms., TotalTime: " << static_cast<UINT>(timer.TotalTime()) << " s.";
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

	pHills->SetCameraMatrix(mCamera * CameraZoom());
	pHills->Update(timer.TotalTime());
	pHills->UpdateConstantBuffers(wnd.GetGraphics(),  wEyePosition, pos, target); //offsetForHillsWithWaves
	pHills->BindAndDrawIndexed(wnd.GetGraphics());
	SetObjectMatrix(offsetForHillsWithWaves);

	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(RenderStates::TransparentBS, blendFactorsZero, 0xffffffff);

	pWaves->SetCameraMatrix(mCamera * CameraZoom());
	pWaves->BindAndDrawIndexed(wnd.GetGraphics());
	pWaves->UpdateScene(timer.TotalTime(), timer.DeltaTime(), wnd.GetGraphics(), wEyePosition);
	pWaves->UpdateVertexConstantBuffer(wnd.GetGraphics());
	SetObjectMatrix(DirectX::XMMatrixIdentity());

}

void App::CreateHillsWithWaves()
{
 	pHills = new Hills(wnd.GetGraphics(), 160.0f, 160.0f, 50u, 50u, false);
	pWaves = new WaveSurface(wnd.GetGraphics());
}

void App::CreateBox()
{
	pBox = new Box(wnd.GetGraphics(), 7.0f, 7.0f, 7.0f, false);
}

void App::DrawBox()
{
	SetObjectMatrix(DirectX::XMMatrixTranslation(0.0f, -5.0f, 0.0f));
	pBox->SetCameraMatrix(mCamera * CameraZoom());
	pBox->Update(timer.TotalTime());
	pBox->UpdateVertexConstantBuffer(wnd.GetGraphics());
	pBox->BindAndDrawIndexed(wnd.GetGraphics());
}

void App::MirrorDemoCreate()
{
	pBox = new Box(wnd.GetGraphics(), 2.5f, 2.5f, 1.5f, true);
	pSkull = new Skull(wnd.GetGraphics(), L"models\\skull.txt");
	pMirrorRoom = new MirrorRoom(wnd.GetGraphics());
}

void App::MirrorDemoDraw()
{
	//SetObjectMatrix(shapes.Get_m_BoxWorld() * shapes.GetCameraOffset());
	SetObjectMatrix(DirectX::XMMatrixIdentity());

// 	pMirrorRoom->UpdateVertexConstantBuffer(wnd.GetGraphics());
	pMirrorRoom->SetCameraMatrix(mCamera * CameraZoom());

	//floor
	pMirrorRoom->UpdateMirrorRoomConstBuffers(wnd.GetGraphics(), 2u);
	pMirrorRoom->BindAndDraw(wnd.GetGraphics(), 6u, 0u);


	//wall
	pMirrorRoom->UpdateMirrorRoomConstBuffers(wnd.GetGraphics(), 0u);
	pMirrorRoom->BindAndDraw(wnd.GetGraphics(), 18u, 6u);


	//skull
// 	SetObjectMatrix(shapes.Get_m_CenterSphere() * DirectX::XMMatrixTranslation(4.0f, 0.0f, -0.5f) *
// 		DirectX::XMMatrixRotationY(DirectX::XM_PI * 0.5f) * shapes.GetCameraOffset());
	SetObjectMatrix(DirectX::XMMatrixTranslation(0.0f, 1.0f, -4.0f));

	pSkull->SetCameraMatrix(DirectX::XMMatrixRotationY(0.6f) * DirectX::XMMatrixScaling(0.3f, 0.3f, 0.3f) * mCamera * CameraZoom());
	pSkull->UpdateVertexConstantBuffer(wnd.GetGraphics());
	pSkull->BindAndDrawIndexed(wnd.GetGraphics());



	// Do not write to render target
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(RenderStates::NoRenderTargetWritesBS, blendFactorsZero, 0xffffffff);
	// Render visible mirror pixels to stencil buffer.
	// Do not write mirror depth to depth buffer at this point, otherwise it will occlude the reflection.
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(RenderStates::MarkMirrorDSS, 1);
	//mirror
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
	for (UINT i = 0; i < 3; i++)
	{
		oldLIghtDirection[i] = pSkull->GetLight(i).direction;
		DirectX::XMFLOAT3 lightDirection = pSkull->GetLight(i).direction;
		DirectX::XMVECTOR lightDir;
		lightDir.m128_f32[0] = lightDirection.x;
		lightDir.m128_f32[1] = lightDirection.y;
		lightDir.m128_f32[2] = lightDirection.z;
		lightDir.m128_f32[0] = 0.0f;
// 		DirectX::XMVECTOR lightDir_n = DirectX::XMVector3Normalize(lightDir);
		DirectX::XMVECTOR reflectedLD = DirectX::XMVector3TransformNormal(lightDir, R);
		DirectX::XMFLOAT3 reflectedLightDirection;
		DirectX::XMStoreFloat3(&reflectedLightDirection, reflectedLD);
		pSkull->SetNewLightDirection(reflectedLightDirection, i);
	}

	

	pSkull->UpdateLightDirection(wnd.GetGraphics());
	//cull clockwise triangles for reflections. IS IT DOING ANYTHING?
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::CullClockwiseRS);
//  	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(RenderStates::WireframeRS);

	// Only draw reflection into visible mirror pixels as marked by the stencil buffer. 
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(RenderStates::DrawReflectionDSS, 1);

	SetObjectMatrix(R * DirectX::XMMatrixTranslation(0.0f, 1.0f, 4.0f));
	pSkull->SetCameraMatrix(DirectX::XMMatrixScaling(0.3f, 0.3f, 0.3f) * mCamera * CameraZoom());
	pSkull->UpdateVertexConstantBuffer(wnd.GetGraphics());
	pSkull->BindAndDrawIndexed(wnd.GetGraphics());
	// Restore default states.
	wnd.GetGraphics().pgfx_pDeviceContext->RSSetState(0);
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetDepthStencilState(0, 0);

	//restore light direction
	for (int i = 0; i < 3; ++i)
	{
		pSkull->SetNewLightDirection(oldLIghtDirection[i], i);
	}
	

	// Draw the mirror to the back buffer as usual but with transparency
	// blending so the reflection shows through.
 	pMirrorRoom->UpdateMirrorRoomConstBuffers(wnd.GetGraphics(), 1u);
	wnd.GetGraphics().pgfx_pDeviceContext->OMSetBlendState(RenderStates::TransparentBS, blendFactorsZero, 0xffffffff);

	pMirrorRoom->BindAndDraw(wnd.GetGraphics(), 6u, 24u);





// 	SetObjectMatrix(DirectX::XMMatrixTranslation(0.0f, 1.3f, -4.0f) *
// 		 shapes.GetCameraOffset());
// 
// 	SetObjectMatrix(DirectX::XMMatrixTranslation(0.0f, 0.0f, -4.0f));
// 	pBox->UpdateVertexConstantBuffer(wnd.GetGraphics());
// 	pBox->SetCameraMatrix(mCamera * CameraZoom());
// 	pBox->BindAndDrawIndexed(wnd.GetGraphics());




}

DirectX::XMMATRIX App::CameraZoom() const noexcept
{
	return DirectX::XMMatrixTranslation(0.0f, -zoom, zoom);
}

DirectX::XMMATRIX App::CalculateProjection() noexcept
{
	SetObjectMatrix(DirectX::XMMatrixIdentity()); //initialize with nothing;
	return   mCamera * DirectX::XMMatrixPerspectiveFovLH(((FOV / 360.0f) * DirectX::XM_2PI) * 0.75f, screenAspect, 0.1f, 1000.0f);

}




void App::ShapesDemoCreateShapes()
{
	pBox = new Box(wnd.GetGraphics(), 1.5f, 1.5f, 2.5f, true);
 	//pGeoSphere = new GeoSphere(wnd.GetGraphics(), 0.5f, 20u);
 	pSkull = new Skull(wnd.GetGraphics(), L"models\\skull.txt");
	pHills = new Hills(wnd.GetGraphics(), 25.0f, 25.0f, 65, 45, true);
	for (int i = 0; i < 10; i++)
	{
		cylinders.push_back(new Cylinder(wnd.GetGraphics(), 0.5f, 0.3f, 3.0f, 20, 20));
	}

	for (size_t i = 0; i < 10; i++)
	{
		geoSpheres.push_back(new GeoSphere(wnd.GetGraphics(), 0.5f, 2u));
	}
}

void App::ShapesDemoDrawShapes()
{
	SetObjectMatrix(shapes.Get_m_BoxWorld() * shapes.GetCameraOffset());
	pBox->UpdateVertexConstantBuffer(wnd.GetGraphics());
	pBox->SetCameraMatrix(mCamera * CameraZoom());
	pBox->BindAndDrawIndexed(wnd.GetGraphics());

	SetObjectMatrix(shapes.Get_m_CenterSphere() * shapes.GetCameraOffset());
	pSkull->SetCameraMatrix(DirectX::XMMatrixScaling(0.3f, 0.3f, 0.3f) * mCamera * CameraZoom() );
	pSkull->UpdateVertexConstantBuffer(wnd.GetGraphics());
	pSkull->BindAndDrawIndexed(wnd.GetGraphics());

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

	mCamera = in_matrix * DirectX::XMMatrixLookAtLH(pos, target, up);

}

App::~App()
{
}
