#include "App.h"

App::App()
	: wnd("Output Window", 800, 600)

{
}

int App::Go()
{
	wnd.ProcessMessages();
	return 0;
}
