#include "App.h"

static int count = 0;

App::App()
	: wnd("Output Window", 800, 600)
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


void App::SendTextToTitle()
{
	std::ostringstream oss;
	oss << "Test string";
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
		oss << "D3D. FPS:" << fps
			<< "; Frame Time: " << ms_per_frame;
		wnd.SetTitle(oss.str().c_str());
		frameCount = 0;
		timeElapsed += 1.0f;
	}
}



void App::DoFrame()
{
	const float c = (cos(timer.TotalTime()));
	timer.Tick();
	CalculateFrameStats();
	wnd.GetGraphics().EndFrame();
	wnd.GetGraphics().ClearBuffer(c, c * 0.2f, c * 0.4f);
}

