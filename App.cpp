#include "App.h"

App::App()
	: wnd("Output Window", 800, 600)

{
}


int App::Go()
{
	while (true)
	{
		Sleep(1);
		timer.Tick();
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}
 		std::ostringstream oss;
 		oss << "time is: " << timer.TotalTime();
		wnd.SetTitle(oss.str().c_str());
	}
	return 0;
}



/*
int App::Go()
{
	timer.Tick();
	while (true)
	{
		Sleep(1);

		//wnd.ProcessMessages();
		if (Window::ProcessMessages())
		{
			Sleep(1);


		}
		std::ostringstream oss;
		oss << "time is: " << timer.TotalTime();
// 		oss << "test";
// 		wnd.SetTitle("hey");
		//wnd.SetTitle(oss.str().c_str());
	}
	return 0;
}

*/