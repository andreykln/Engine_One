#pragma once
#include "Window.h"
#include "Timer.h"
#include "DrawableBase.h"
#include "Shaders.h"
// #include "Camera.h"
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
	void CreateHillsWithGPUWaves();
	void DrawHillsWithGPUWaves();
	void CreateBilateralHillsBlur();
	void DrawBilateralHillsBlur();
	void CreateGaussBlur();
	void DrawGaussBlur();
	void CreateBox();
	void DrawBox();
	void CreateDepthComplexityStencil();
	void DrawDepthComplexityStencil();

	//shadow map
	void DrawSceneToShadowMap();
	void CreateShadowMapDemo();
	void DrawShadowMapDemo();

	void CreateBezierPatchTess();
	void DrawBezierPatchTess();
	void InstancingCreate();
	void DrawInstancingDraw();
	void CreateTerrain();
	void DrawTerrain();
	DirectX::XMMATRIX GetViewProjectionCamera();


private:
	DirectX::XMMATRIX viewProjectionMatrix;

	const float blendFactorsZero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	const float blendFactorsOne[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	Window wnd;
	Timer timer;
	RenderStates rStates;
	Shape* pShapes;

	Camera camera;
	CubeMapCamera* pCMCamera;
	Shaders* pShaders = nullptr;

	ShadowMapGen* pShadowMap = nullptr;
	
	ParticleSystem* pParticle = nullptr;
	ParticleSystem* pParticleRain = nullptr;
	ParticleSystem* pParticleExplosion = nullptr;
	ParticleSystem* pParticleFountain = nullptr;
	Terrain* pTerrain = nullptr;
	InstancedSkull* pInstancedSkulls = nullptr;
	WaveSurfaceGPU* pWaveSurfaceGPU = nullptr;
	GaussianBlur* pGaussianBlur = nullptr;
	DepthComplexity* pDepthArr[3];
 	Box* pBox = nullptr;
	Box* pDisplacementMappingBox = nullptr;
	Cylinder* pInstancedCylinder = nullptr;
	GeoSphere* pInstancedGeoSphere = nullptr;
 	GeoSphere* pGeoSphere = nullptr;
  	Hills* pHills = nullptr;
 	Skull* pSkull = nullptr;
	QuadTessellation* pQuadTess = nullptr;
	Sky* pSky = nullptr;
  	ShapesDemo shapes;

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

