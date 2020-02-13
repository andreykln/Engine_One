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
		Sleep(1);
		timer.Tick();
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}
		CalculateFrameStats();
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
	if (timer.TotalTime() - timeElapsed >= 1.0f)
	{
		float fps = static_cast<float>(frameCount);
		float ms_per_frame = 1000.0f / fps;
		std::ostringstream oss;
		oss << "D3D. FPS:" << fps
			<< "; Frame Time: " << ms_per_frame
			<< "; Total time " << timer.TotalTime();
		wnd.SetTitle(oss.str().c_str());
	}


}

