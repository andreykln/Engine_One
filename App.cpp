#include "App.h"


App::App()
	: wnd("Output Window", resolution_width, resolution_height)
{
	for (size_t i = 0; i < 1; i++)
	{
		cubes.push_back(std::make_unique<Cube>(wnd.GetGraphics()));
	}

	wnd.GetGraphics().SetProjection(CalculateProjection());
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
	while (!wnd.mouse.IsEmpty())
	{
		const Mouse::Event e = wnd.mouse.Read();
		switch (e.GetType())
		{
			case Mouse::Event::Type::MWheelUp:
			{
				axis_z += camera_move_step;
				if (axis_z > 8.0f)
				{
					axis_z = 8.0f;
				}
			}
			break;

			case Mouse::Event::Type::MWheelDown:
			{
				axis_z -= camera_move_step;
				if (axis_z < 1.0f)
				{
					axis_z = 1.0f;
				}
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
		oss << "D3D. FPS:" << fps
			<< "; Frame Time: " << ms_per_frame;
		wnd.SetTitle(oss.str().c_str());
		frameCount = 0;
		timeElapsed += 1.0f;
	}
}



void App::CameraMove()
{
	const auto charPress = wnd.kbd.ReadChar();



	switch (charPress)
	{
	case  'a': case 'A':
	{
		axis_x += camera_move_step;
	}
	break;
	case 'd': case 'D':
	{
		axis_x -= camera_move_step;
	}
	break;
	case 'w': case 'W':
	{
		axis_y += camera_move_step;
	}
	break;
	case 's': case 'S':
	{
		axis_y -= camera_move_step;
	}
	break;


	}


}

void App::DoFrame()
{
	const float c = abs((sin(timer.TotalTime())));
	timer.Tick();
// 	TwoTestCubes();
	for (auto& b : cubes)
	{
		b->BindAndDraw(wnd.GetGraphics());
	}
	CalculateFrameStats();
	//CameraMove();
	//ScrollWheelCounter();

	//DebugTextToTitle();
	wnd.GetGraphics().EndFrame();
	wnd.GetGraphics().ClearBuffer(c * 0.2f, c * 0.2f, c * 0.5f);
}

void App::TwoTestCubes() noexcept
{
	wnd.GetGraphics().TestDrawing(timer.TotalTime(), 0.0, 0.0f, 4.0f);
	wnd.GetGraphics().TestDrawing((timer.TotalTime() * 0.5f), axis_x, axis_y, axis_z);

}

DirectX::XMMATRIX App::CalculateProjection() noexcept
{
	
	return		DirectX::XMMatrixTranslation(0.0f, 0.0f, 4.0f) *
		DirectX::XMMatrixPerspectiveFovLH(FOV, screenAspect, 0.1f, 100.0f);

}

