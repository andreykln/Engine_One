#pragma once
#include "Window.h"
#include "Timer.h"
#include <cmath>
#include <sstream>
extern const short resolution_width;
extern const short resolution_height;

class App
{
public:
	App();
	int Go();
	void DebugTextToTitle();
	void ScrollWheelCounter();
	void CalculateFrameStats();
	void CameraMove();
	void DoFrame();
private:
	Window wnd;
	Timer timer;
	float colors[4]{};
// 	float zoom_count = 4.0f;
	const float camera_move_step = 0.01f;

	float axis_x = 0.0f;
	float axis_y = 0.0f;
	float axis_z = 4.0f;


};

