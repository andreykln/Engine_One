#include "App.h"

//static int count = 0;

App::App()
	: wnd("Output Window", resolution_width, resolution_height)
{
	
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
				count += 0.1f;
				if (count > 8.0f)
				{
					count = 8.0f;
				}
			}
			break;

			case Mouse::Event::Type::MWheelDown:
			{
				count -= 0.1f;
				if (count < 1.0f)
				{
					count = 1.0f;
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



void App::DoFrame()
{
	const float c = abs((sin(timer.TotalTime())));
	timer.Tick();
	wnd.GetGraphics().TestDrawing(timer.TotalTime(), 4.0);
	wnd.GetGraphics().TestDrawing((timer.TotalTime() * 0.5f), count);


	CalculateFrameStats();
	ScrollWheelCounter();

	//DebugTextToTitle();
	wnd.GetGraphics().EndFrame();
	wnd.GetGraphics().ClearBuffer(c * 0.2f, c * 0.2f, c * 0.5f);
}

