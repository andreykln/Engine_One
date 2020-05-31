
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "App.h"

INT test = 0;
App::App()
	: wnd("Output Window", resolution_width, resolution_height)
{
// 	pBox = new Box(wnd.GetGraphics(), 1.0f, 1.0f, 1.0f);
// 	pBox1 = new Box(wnd.GetGraphics(), 1.0f, 1.0f, 1.0f);

	//boxandCyl = new BoxAndCylinder(wnd.GetGraphics());
 	CreateHillsWithWaves();
 	wnd.GetGraphics().SetProjection(CalculateProjection());
}

App::~App()
{
}

void App::DoFrame()
{
// 	const float c = abs((sin(timer.TotalTime())));
	timer.Tick();


//  	SetObjectMatrix(DirectX::XMMatrixRotationRollPitchYaw(0.0f, timer.TotalTime(), 0.0f));
	DrawHillsWithWaves();
//  	pBox->SetCameraMatrix(mCamera);
// 	pBox->UpdateVertexConstantBuffer(wnd.GetGraphics());
// 	pBox->BindAndDraw(wnd.GetGraphics());


// 	SetObjectMatrix(DirectX::XMMatrixRotationRollPitchYaw(0.0f, -timer.TotalTime(), 0.0f));
// 	pBox1->SetCameraMatrix(mCamera * DirectX::XMMatrixRotationRollPitchYaw(0.0f, -timer.TotalTime(), 0.0f));
// 	pBox1->UpdateVertexConstantBuffer(wnd.GetGraphics());
// 	pBox1->BindAndDraw(wnd.GetGraphics());
// 


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
	oss << "X: " << wnd.mouse.GetPosX() << " Y: " << wnd.mouse.GetPosY() << " Theta: " << wnd.GetTheta() <<
		 " Radius: " << wnd.GetRadius() << " Phi: " << wnd.GetPhi();
	wnd.SetTitle(oss.str().c_str());
}

void App::ScrollWheelCounter()
{
// 	while (!wnd.mouse.IsEmpty())
// 	{
// 		const Mouse::Event e = wnd.mouse.Read();
// 		switch (e.GetType())
// 		{
// 			case Mouse::Event::Type::MWheelUp:
// 			{
// 				axis_z += camera_move_step;
// 				if (axis_z > 8.0f)
// 				{
// 					axis_z = 8.0f;
// 				}
// 			}
// 			break;
// 
// 			case Mouse::Event::Type::MWheelDown:
// 			{
// 				axis_z -= camera_move_step;
// 				if (axis_z < 1.0f)
// 				{
// 					axis_z = 1.0f;
// 				}
// 			}
// 			break;
// 		}
// 	}
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
		oss << "D3D. FPS:" << fps
			<< "; Frame Time: " << ms_per_frame;
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
// 	pWaves->SetCameraMatrix(mCamera);
// 	pWaves->BindAndDraw(wnd.GetGraphics());
// 	pWaves->UpdateScene(timer.TotalTime(), timer.DeltaTime(), wnd.GetGraphics());
// 	pWaves->UpdateVertexConstantBuffer(wnd.GetGraphics());
// 	SetObjectMatrix(DirectX::XMMatrixIdentity());
// 
	pHills->SetCameraMatrix(mCamera /** DirectX::XMMatrixRotationY(timer.TotalTime())*/);
// 	SetObjectMatrix(offsetForHillsWithWaves);
	pHills->Update(timer.TotalTime());
	pHills->UpdateConstantBuffers(wnd.GetGraphics(),  wEyePosition, pos, target); //offsetForHillsWithWaves
	pHills->BindAndDraw(wnd.GetGraphics());
	SetObjectMatrix(offsetForHillsWithWaves);

}

void App::CreateHillsWithWaves()
{
 	pHills = new Hills(wnd.GetGraphics(), 160.0f, 160.0f, 50u, 50u, false);
	pWaves = new WaveSurface(wnd.GetGraphics());
}

DirectX::XMMATRIX App::CalculateProjection() noexcept
{
	SetObjectMatrix(DirectX::XMMatrixIdentity()); //initialize with nothing;
	return   mCamera * DirectX::XMMatrixPerspectiveFovLH(((FOV / 360.0f) * DirectX::XM_2PI) * 0.75f, screenAspect, 0.1f, 100.0f);

}


float App::X_Generate(size_t& i, float& current_X_Axis)
{
	if (i != 0)
	{
		current_X_Axis += 1.8f;
		return current_X_Axis;
	}
	return axis_x;
}

float App::Y_Generate(size_t& j, float& current_Y_Axis)
{
	if (j != 0)
	{
		current_Y_Axis -= 1.8f;
		return current_Y_Axis;
	}
	return axis_y;

}
float App::Z_Generate(size_t& k, float& current_Z_Axis)
{
	if (k != 0)
	{
		current_Z_Axis -= 1.8f;
		return current_Z_Axis;
	}
	return axis_z;
}

void App::ShapesDemoCreateShapes()
{
// 	pBox = new Box(wnd.GetGraphics(), 1.0f, 1.0f, 0.5f);
// 	pSphere = new Sphere(wnd.GetGraphics(), 0.5f, 20, 20);
// 	pHills = new Hills(wnd.GetGraphics(), 25.0f, 25.0f, 65, 45, true);
// 	for (int i = 0; i < 10; i++)
// 	{
// 		cylinders.push_back(new Cylinder(wnd.GetGraphics(), 0.5f, 0.3f, 3.0f, 20, 20));
// 	}
// 
// 	for (size_t i = 0; i < 10; i++)
// 	{
// 		geoSpheres.push_back(new GeoSphere(wnd.GetGraphics(), 0.5f, 2u));
// 	}
}

void App::ShapesDemoDrawShapes()
{
// 	pBox->SetMatrix(shapes.Get_m_BoxWorld() * shapes.GetCameraOffset());
// 	pBox->BindAndDraw(wnd.GetGraphics());
// 
// 	pSphere->SetMatrix(shapes.Get_m_CenterSphere() * shapes.GetCameraOffset());
// 	pSphere->BindAndDraw(wnd.GetGraphics());
// 
// 	pHills->SetMatrix(shapes.Get_m_GridWorld() * shapes.GetCameraOffset());
// 	pHills->BindAndDraw(wnd.GetGraphics());
// 
// 	for (auto& x : cylinders)
// 	{
// 		x->SetMatrix(*(shapes.GetCylinderWorldArray())++ /** DirectX::XMMatrixRotationY(timer.TotalTime())*/ * shapes.GetCameraOffset());
// 		x->BindAndDraw(wnd.GetGraphics());
// // 		x->SetMatrix();
// 	}
// 	shapes.GetCylinderWorldArray() -= 10; //reset array position
// 
// 	for (auto& x : geoSpheres)
// 	{
// 		x->SetMatrix(*(shapes.GetSphereWorldArray())++ * shapes.GetCameraOffset());
// 		x->BindAndDraw(wnd.GetGraphics());
// 	}
// 	shapes.GetSphereWorldArray() -= 10; //reset array position
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
