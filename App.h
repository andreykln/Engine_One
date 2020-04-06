#pragma once
#include "Window.h"
#include "Timer.h"
#include "Cube.h"
#include <vector>
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
	void TwoTestCubes() noexcept;
	DirectX::XMMATRIX CalculateProjection() noexcept;

	//Axis step generation
	float X_Generate(size_t& i, float& current_X_Axis);
	float Y_Generate(size_t& j, float& current_Y_Axis);
	float Z_Generate(size_t& k, float& current_Z_Axis);

private:
	const float FOV = DirectX::XM_PI / 4.0f;
	const float screenAspect = float(resolution_width) / float(resolution_height);
	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixIdentity();
	std::vector<std::unique_ptr<Cube>> cubes;
	Window wnd;
	Timer timer;
	float colors[4]{};
	const float camera_move_step = 0.05f;

	const float axis_x = 0.0f;
	const float axis_y = 0.0f;
	const float axis_z = 5.0f;

// 	const float axis_x = -15.0f;
// 	const float axis_y = 15.0f;
// 	const float axis_z = 0.0f;

};

