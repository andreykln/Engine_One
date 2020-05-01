#include "App.h"

App::App()
	: wnd("Output Window", resolution_width, resolution_height)
{
// 	wave.push_back(std::make_unique<WaveSurface>(wnd.GetGraphics()) );
	box.push_back(std::make_unique<Box>(wnd.GetGraphics(), 0.23f, 0.5f, 0.3f));
	wnd.GetGraphics().SetProjection(CalculateProjection());
}

void App::DoFrame()
{
// 	const float c = abs((sin(timer.TotalTime())));
	timer.Tick();

// 	for (auto& x : wave)
// 	{
// 		x->UpdateScene(timer.TotalTime(), timer.DeltaTime(), wnd.GetGraphics());
// 		x->SetMatrix(offset);
// 		x->BindAndDraw(wnd.GetGraphics());
// 	}

	for (auto& x : box)
	{
		x->SetMatrix(DirectX::XMMatrixRotationX(timer.TotalTime()) * offset);
		x->BindAndDraw(wnd.GetGraphics());
	}



	CalculateFrameStats();

	//DebugTextToTitle();
	wnd.GetGraphics().EndFrame();
	wnd.GetGraphics().ClearBuffer(0.5f, 0.7f, 0.7f);
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
	wnd.GetGraphics().TestDrawing((timer.TotalTime() * 0.5f), axis_x, axis_y, axis_z);
}



void App::Terrain()
{
// 	const float depth = 160.0f;
// 	const float width = 160.0f;
// 	const UINT m = 50;
// 	const UINT n = 50;
// 	grid.push_back(std::make_unique<Hills>(wnd.GetGraphics(), depth, width, m, n));
}

DirectX::XMMATRIX App::CalculateProjection() noexcept
{
	return 	DirectX::XMMatrixPerspectiveFovLH(FOV, screenAspect, 0.1f, 100.0f);
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
// 	box.push_back(std::make_unique<Box>(wnd.GetGraphics(), 1.0f, 1.0f, 0.5f));
// 	sphere.push_back(std::make_unique<Sphere>(wnd.GetGraphics(), 0.5f, 20, 20));
// 	grid.push_back(std::make_unique<Hills>(wnd.GetGraphics(), 25.0f, 25.0f, 65, 45));
// 
// 	for (int i = 0; i < 10; i++)
// 	{
// 		cylinder.push_back(std::make_unique<Cylinder>(wnd.GetGraphics(), 0.5f, 0.3f, 3.0f, 20, 20));
// 	}
// 
// 	for (size_t i = 0; i < 10; i++)
// 	{
// 		geoSphere.push_back(std::make_unique<GeoSphere>(wnd.GetGraphics(), 0.5f, 2u));
// 	}
}

void App::ShapesDemoDrawShapes()
{
// 	box[0]->SetMatrix(shapes.Get_m_BoxWorld() * shapes.GetCameraOffset());
// 	box[0]->BindAndDraw(wnd.GetGraphics());
// 
// 	sphere[0]->SetMatrix(shapes.Get_m_CenterSphere() * shapes.GetCameraOffset());
// 	sphere[0]->BindAndDraw(wnd.GetGraphics());
// 
// 	grid[0]->SetMatrix(shapes.Get_m_GridWorld() * shapes.GetCameraOffset());
// 	grid[0]->BindAndDraw(wnd.GetGraphics());
// 
// 	for (auto& x : cylinder)
// 	{
// 		x->SetMatrix(*(shapes.GetCylinderWorldArray())++ * shapes.GetCameraOffset());
// 		x->BindAndDraw(wnd.GetGraphics());
// 	}
// 	shapes.GetCylinderWorldArray() -= 10; //reset array position
// 
// 	for (auto& x : geoSphere)
// 	{
// 		x->SetMatrix(*(shapes.GetSphereWorldArray())++ * shapes.GetCameraOffset());
// 		x->BindAndDraw(wnd.GetGraphics());
// 	}
// 	shapes.GetSphereWorldArray() -= 10; //reset array position
}
