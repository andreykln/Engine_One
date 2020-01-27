#include "App.h"

App::App()
	: wnd("Output Window", 800, 600)

{
}

int App::Go()
{
	timer.Tick();

	while (true)
	{
		//wnd.ProcessMessages();
		Window::ProcessMessages();
		Sleep(1);

		

		std::ostringstream oss;
		oss << "time is: " << timer.TotalTime();
// 		oss << "test";
// 		wnd.SetTitle("hey");
		wnd.SetTitle(oss.str().c_str());
	}
	return 0;
}
