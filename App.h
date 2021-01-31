#pragma once
#include "Window.h"
#include "Timer.h"
#include "Hills.h"
#include "Box.h"
#include "ShapesDemo.h"
#include "Pyramid.h"
#include "GeoSphere.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "HillsAndWaves.h"
#include "Skull.h"
#include "BoxAndCylinder.h"
#include "MirrorRoom.h"
#include "DepthComplexity.h"
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
	~App();
	void DoFrame();
	int Go();
	void DebugTextToTitle();
	void ScrollWheelCounter();
	void CalculateFrameStats();
	void CameraMove();
	void TwoTestCubes() noexcept;
	void DrawHillsWithWaves();
	void CreateHillsWithWaves();
	void CreateBox();
	void DrawBox();
	void MirrorDemoCreate();
	void MirrorDemoDraw();
	void LightningCreate();
	void LightningDraw();
	DirectX::XMMATRIX CameraZoom() const noexcept;
	DirectX::XMMATRIX CalculateProjection() noexcept;
private:
	const float blendFactorsZero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	const float blendFactorsOne[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMMATRIX mirroredSkull = DirectX::XMMatrixTranslation(0.0f, 1.0f, -4.0f);

	Window wnd;
	Timer timer;
	RenderStates rStates;
	DepthComplexity* pDepth = nullptr;
// 	BoxAndCylinder* boxandCyl = nullptr;
 	Box* pBox = nullptr;
 	Cylinder* pCylinder = nullptr;
 	std::vector<Cylinder*> cylinders;
 	GeoSphere* pGeoSphere = nullptr;
 	std::vector<GeoSphere*> geoSpheres;
//  	Sphere* pSphere = nullptr;
  	Hills* pHills = nullptr;
	MirrorRoom* pMirrorRoom = nullptr;
 	Skull* pSkull = nullptr;
 	WaveSurface* pWaves = nullptr;



// 	Pyramid* pPyramid = nullptr;
  	ShapesDemo shapes;

	const DirectX::XMMATRIX offsetForHills = DirectX::XMMatrixTranslation(0.0f, -4.0f, 10.0f);
	const DirectX::XMMATRIX offsetForWavesWithHills = DirectX::XMMatrixTranslation(0.0f, -8.0f, 10.0f);
	DirectX::XMMATRIX offsetForHillsWithWaves = DirectX::XMMatrixTranslation(0.0f, -4.0f, 10.0f);
	const DirectX::XMMATRIX offsettest = DirectX::XMMatrixTranslation(-0.6f, 0.0f, 0.0f);

	void ShapesDemoCreateShapes();
	void ShapesDemoDrawShapes();
	//for movement around object's matrix
	void SetObjectMatrix(DirectX::XMMATRIX in_matrix);
	DirectX::XMFLOAT3 wEyePosition;
	DirectX::XMMATRIX mCamera;
	DirectX::XMMATRIX objectMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMVECTOR pos;
	DirectX::XMVECTOR target;
	DirectX::XMVECTOR up;

	int test = 0;
	const float FOV = 90.0f;
	const float screenAspect = float(resolution_width) / float(resolution_height);
	float zoom = 0.0f;
	float colors[4]{};
	const float camera_move_step = 0.05f;
	const float axis_x = -10.0f;
	const float axis_y = 10.0f;
	const float axis_z = 5.0f;

	float millisecCounter = 0.0f;
	float millisecElapsed = 0.0f;
	const double sixtythOfASecond = 1000.0 / 60.0;

};

