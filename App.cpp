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
	}
	return 0;
}


void App::SendTextToTitle()
{
	std::ostringstream oss;

	
	wnd.SetTitle(oss.str().c_str());
}

