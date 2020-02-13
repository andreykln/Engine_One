#pragma once
#include "Window.h"
#include "Timer.h"
#include <sstream>
class App
{
public:
	App();
	int Go();
	void SendTextToTitle();
	void CalculateFrameStats();
private:
	Window wnd;
	Timer timer;

};

