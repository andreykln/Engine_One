#pragma once
#include "Window.h"
#include "Timer.h"
#include "Hills.h"
#include "Box.h"
#include "ShapesDemo.h"
#include "GeoSphere.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "LoadModelFromTXT.h"

#include "WaveSurface.h"
#include <vector>
#include <cmath>
#include <sstream>

extern const short resolution_width;
extern const short resolution_height;
class App
{
public:
	App();
	void DoFrame();
	int Go();
	void DebugTextToTitle();
	void ScrollWheelCounter();
	void CalculateFrameStats();
	void CameraMove();
	void TwoTestCubes() noexcept;
	void Terrain();
	DirectX::XMMATRIX CalculateProjection() noexcept;

	//Axis step generation for cubes
	float X_Generate(size_t& i, float& current_X_Axis);
	float Y_Generate(size_t& j, float& current_Y_Axis);
	float Z_Generate(size_t& k, float& current_Z_Axis);
private:
	const float FOV = DirectX::XM_PI / 4.0f;
	const float screenAspect = float(resolution_width) / float(resolution_height);
// 	std::vector<std::unique_ptr<Hills>> grid;
// 	std::vector<std::unique_ptr<Cylinder>> cylinder;
// 	std::vector<std::unique_ptr<Sphere>> sphere;
// 	std::vector<std::unique_ptr<GeoSphere>> geoSphere;
	std::vector<Box*> box;
// 	std::vector<std::unique_ptr<LoadModelFromTXT>> txtModel;
// 	std::vector<std::unique_ptr<WaveSurface>> wave;

// 	ShapesDemo shapes;

	DirectX::XMMATRIX offset = DirectX::XMMatrixTranslation(0.f, -4.0f, 10.0f);

	void ShapesDemoCreateShapes();
	void ShapesDemoDrawShapes();
	Window wnd;
	Timer timer;
	float colors[4]{};
	const float camera_move_step = 0.05f;

	const float axis_x = -10.0f;
	const float axis_y = 10.0f;
	const float axis_z = 5.0f;
};

