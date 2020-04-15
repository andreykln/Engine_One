#include "App.h"


App::App()
	: wnd("Output Window", resolution_width, resolution_height)
{
	float a = 0.5f;


	//Terrain();
// 	cubes.push_back(std::make_unique<Cube>(wnd.GetGraphics(), a, a, a));

	cylinder.push_back(std::make_unique<Cylinder>(wnd.GetGraphics(),0.0f));
	wnd.GetGraphics().SetProjection(CalculateProjection());
}

void App::DoFrame()
{
	const float c = abs((sin(timer.TotalTime())));
	timer.Tick();
	for (auto& h : cylinder)
	{
		h->BindAndDraw(wnd.GetGraphics());
		h->Update(timer.TotalTime() * 0.5);
	}
// 	for (auto& h : cubes)
// 	{
// 		h->BindAndDraw(wnd.GetGraphics());
// 		h->Update(timer.TotalTime() * 0.5);
// 	}
	CalculateFrameStats();

	//DebugTextToTitle();
	wnd.GetGraphics().EndFrame();
	wnd.GetGraphics().ClearBuffer(c * 0.2f, c * 0.2f, c * 0.5f);
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
	oss << cubes.size();
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
// 	const auto charPress = wnd.kbd.ReadChar();
// 
// 
// 
// 	switch (charPress)
// 	{
// 	case  'a': case 'A':
// 	{
// 		axis_x += camera_move_step;
// 	}
// 	break;
// 	case 'd': case 'D':
// 	{
// 		axis_x -= camera_move_step;
// 	}
// 	break;
// 	case 'w': case 'W':
// 	{
// 		axis_y += camera_move_step;
// 	}
// 	break;
// 	case 's': case 'S':
// 	{
// 		axis_y -= camera_move_step;
// 	}
// 	break;
// 
// 
// 	}
// 

}



void App::TwoTestCubes() noexcept
{
	wnd.GetGraphics().TestDrawing(timer.TotalTime(), 0.0, 0.0f, 4.0f);
	wnd.GetGraphics().TestDrawing((timer.TotalTime() * 0.5f), axis_x, axis_y, axis_z);

}

void App::GenerateCubeGrid(size_t dimension, std::vector<std::unique_ptr<Cube>>& cubes)
{
	float current_X_Axis = axis_x;
	float current_Y_Axis = axis_y;
	float current_Z_Axis = axis_z;
	for (size_t i = 0; i < dimension; i++)
	{
		float x = X_Generate(i, current_X_Axis);
		for (size_t j = 0; j < dimension; j++)
		{
			float y = Y_Generate(j, current_Y_Axis);
			for (size_t k = 0; k < dimension; k++)
			{
				float z = Z_Generate(k, current_Z_Axis);
				cubes.push_back(std::make_unique<Cube>(wnd.GetGraphics(), x, y, z));
				if (k == (dimension - 1))
				{
					current_Z_Axis = axis_z;
				}
			}
			if (j == (dimension - 1))
			{
				current_Y_Axis = axis_y;
			}
		}
	}

}

void App::Terrain()
{
	const float depth = 160.0f;
	const float width = 160.0f;
	const UINT m = 50;
	const UINT n = 50;
	hills.push_back(std::make_unique<Hills>(wnd.GetGraphics(), depth, width, m, n));
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
