#pragma once
#include "Window.h"
#include "Timer.h"
#include <cmath>
#include <sstream>
class App
{
public:
	App();
	int Go();
	void DebugTextToTitle();
	void CalculateFrameStats();
	void DoFrame();
private:
	Window wnd;
	Timer timer;
	float colors[4]{};

};

