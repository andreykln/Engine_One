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
	const float colors[4]{ 0.5f, 0.5f, 0.2f };

};

