#pragma once
#include "Window.h"
#include "Timer.h"
#include "DrawableBase.h"
// #include "Shaders.h"
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
	void CreateDepthComplexityStencil();
	void DrawDepthComplexityStencil();

	//shadow map
	void DrawSceneToShadowMap();
	void CreateShadowMapDemo();
	void DrawShadowMapDemo();

	//SSAO
	void DrawNormalMap(DirectX::XMMATRIX viewProjectionMatrix);

	void CreateBezierPatchTess();
	void DrawBezierPatchTess();
	void InstancingCreate();
	void DrawInstancingDraw();
	void CreateTerrain();
	void DrawTerrain();
	DirectX::XMMATRIX GetViewProjectionCamera();

	void SetDefaultRTVAndViewPort();

private:
	DirectX::XMMATRIX viewProjectionMatrix;
	CBufferNames cbNames;

	const float blendFactorsZero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	const float blendFactorsOne[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	Window wnd;
	Timer timer;
	Shape* pShapes;
	ID3D11DeviceContext* pDC = nullptr;
	Camera camera;
	CubeMapCamera* pCMCamera;

	ShadowMapGen* pShadowMap = nullptr;
	DisplacementWaves* pDispWaves = nullptr;
	SSAO* pSSAO = nullptr;

	ParticleSystem* pParticle = nullptr;
	ParticleSystem* pParticleRain = nullptr;
	ParticleSystem* pParticleExplosion = nullptr;
	ParticleSystem* pParticleFountain = nullptr;
	Terrain* pTerrain = nullptr;
	InstancedSkull* pInstancedSkulls = nullptr;
	WaveSurfaceGPU* pWaveSurfaceGPU = nullptr;
	GaussianBlur* pGaussianBlur = nullptr;
	DepthComplexity* pDepthArr[3];
	Box* pDisplacementMappingBox = nullptr;
	//NEW ARCH
	//for empty pass to constant buffers
	const DirectX::XMMATRIX ID = DirectX::XMMatrixIdentity();
	Cylinder* pCylinder = nullptr;
	Hills* pPlate = nullptr;
	Box* pBox = nullptr;
	GeoSphere* pGeoSphere = nullptr;
	UINT stride = 0u;
	UINT offset = 0u;
		////////////
	Cylinder* pInstancedCylinder = nullptr;
	GeoSphere* pInstancedGeoSphere = nullptr;
  	Hills* pHills = nullptr;
 	Skull* pSkull = nullptr;
	QuadTessellation* pQuadTess = nullptr;
	Sky* pSky = nullptr;
  	ShapesDemo shapes;

	UINT stencilRef = 0;

	const float screenAspect = float(resolution_width) / float(resolution_height);
	float zoom = DirectX::XM_PI * 0.25f;
	float colors[4]{ 0.0392f, 0.0392f, 0.17254f, 1.0f};
	const float camera_move_step = 0.05f;
	const float axis_x = -10.0f;
	const float axis_y = 10.0f;
	const float axis_z = 5.0f;

	float millisecCounter = 0.0f;
	float millisecElapsed = 0.0f;
	const double sixtythOfASecond = 1000.0 / 60.0;

};

