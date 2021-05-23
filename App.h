#pragma once
#include "Window.h"
#include "Timer.h"
#include "DrawableBase.h"
#include "Shaders.h"
#include "Camera.h"
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
private:
	void CalculateFrameStats();
	void CreateHillsWithWavesAllLight();
	void DrawHillsWithWavesAllLight();
	void CreateHillsWithWaves();
	void DrawHillsWithWaves();
	void CreateHillsWithGPUWaves();
	void DrawHillsWithGPUWaves();
	void CreateBilateralHillsBlur();
	void DrawBilateralHillsBlur();
	void CreateGaussBlur();
	void DrawGaussBlur();
	void CreateBox();
	void DrawBox();
	void CreateMirror();
	void DrawMirror();
	void CreateLightning();
	void DrawLightning();
	void CreateDepthComplexityStencil();
	void DrawDepthComplexityStencil();
	void CreateShapes();
	void DrawShapes();
	void CreateBezierPatchTess();
	void DrawBezierPatchTess();
	DirectX::XMMATRIX GetViewProjectionCamera();


// 	DirectX::XMMATRIX CameraZoom() const noexcept;
// 	DirectX::XMMATRIX GetPerspectiveProjection(float in_FOV) noexcept;
private:
	DirectX::XMMATRIX viewProjectionMatrix;

	const float blendFactorsZero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	const float blendFactorsOne[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMMATRIX mirroredSkull = DirectX::XMMatrixTranslation(0.0f, 5.0f, -20.0f);

	Window wnd;
	Timer timer;
	RenderStates rStates;
	Shape* pShapes;

	Camera camera;

	ShaderPicker picker = LightAndTexture_VS_PS;
	Shaders* pShaders = nullptr;
	
	WaveSurfaceGPU* pWaveSurfaceGPU = nullptr;
	GaussianBlur* pGaussianBlur = nullptr;
	DepthComplexity* pDepthArr[3];
 	Box* pBox = nullptr;
	TreeBillboard* pBillboards = nullptr;
 	Cylinder* pCylinder = nullptr;
 	std::vector<Cylinder*> cylinders;
 	GeoSphere* pGeoSphere = nullptr;
 	std::vector<GeoSphere*> geoSpheres;
//  Sphere* pSphere = nullptr;
  	Hills* pHills = nullptr;
	MirrorRoom* pMirrorRoom = nullptr;
 	Skull* pSkull = nullptr;
 	WaveSurface* pWaves = nullptr;
	QuadTessellation* pQuadTess = nullptr;


// 	Pyramid* pPyramid = nullptr;
  	ShapesDemo shapes;

// 	const DirectX::XMMATRIX offsetForHills = DirectX::XMMatrixTranslation(0.0f, -4.0f, 10.0f);
// 	const DirectX::XMMATRIX offsetForWavesWithHills = DirectX::XMMatrixTranslation(0.0f, -8.0f, 10.0f);
// 	DirectX::XMMATRIX offsetForHillsWithWaves = DirectX::XMMatrixTranslation(0.0f, -4.0f, 0.0f);

	//Camera from Frank Luna's book
// 	void SetObjectMatrix(DirectX::XMMATRIX in_matrix);
	//from LearnOpenGL
// 	float lastX = 0.0f;
// 	float lastY = 0.0f;
// 	float yaw = 45.5f;
// 	float pitch = 0.0f;
// 	bool firstMouse = true;
// 	float GetYaw();
// 	float GetPitch();



// 	DirectX::XMFLOAT3 wEyePosition;
// 	DirectX::XMMATRIX mCamera;
// 	DirectX::XMMATRIX objectMatrix = DirectX::XMMatrixIdentity();
// 	DirectX::XMVECTOR pos;
// 	DirectX::XMVECTOR target;
// 	DirectX::XMVECTOR up;

	UINT stencilRef = 0;

	const float screenAspect = float(resolution_width) / float(resolution_height);
	float zoom = DirectX::XM_PI * 0.25f;
	float colors[4]{};
	const float camera_move_step = 0.05f;
	const float axis_x = -10.0f;
	const float axis_y = 10.0f;
	const float axis_z = 5.0f;

	float millisecCounter = 0.0f;
	float millisecElapsed = 0.0f;
	const double sixtythOfASecond = 1000.0 / 60.0;

};

