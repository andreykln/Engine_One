#pragma once
#include "Window.h"
#include "Timer.h"
#include "DrawableBase.h"
// #include "M3dLoader.h"
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
	void CreateComputeShaderWaves();
	void DrawComputeShaderWaves();
	//shadow map
	void DrawSceneToShadowMap();
	void CreateShadowMapDemo();
	void DrawShadowMapDemo();
	//SSAO
	void DrawNormalMap();
	void CreateTerrain();
	void DrawTerrain();
	//Temple scene, m3d test
	void CreateTempleScene();
	void DrawTempleScene();

	void CreateAndBindSkybox();
	void DrawSkyBox();

	void LoadModelToMemory(const std::string& filepath, const std::string& modelName);

	DirectX::XMMATRIX GetViewProjectionCamera();
	void SetDefaultRTVAndViewPort();

	//animation
	void SetAnimationKeyframes();
	void UpdateAnimation();

private:
	//////////////////////////////////////////////////////////////////////////
	float mAnimTimePos = 0.0f;
	DirectX::XMMATRIX skullNewWorld;
	BoneAnimation skullAnimation;

	//////////////////////////////////////////////////////////////////////////
	DirectX::XMMATRIX viewProjectionMatrix;
	CBufferNames cbNames;
	M3dModelNames m3dNames;
	TempleDemoWorlds templeWorlds;
	const float blendFactorsZero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	const float blendFactorsOne[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	Window wnd;
	Timer timer;
	ID3D11DeviceContext* pDC = nullptr;
	Camera camera;
	M3dLoader* m3dLoad = nullptr;
	ShadowMapGen* pShadowMap = nullptr;
	DisplacementWaves* pDispWaves = nullptr;
	SSAO* pSSAO = nullptr;

	ParticleSystem* pParticle = nullptr;
	ParticleSystem* pParticleRain = nullptr;
	ParticleSystem* pParticleExplosion = nullptr;
	ParticleSystem* pParticleFountain = nullptr;
	Terrain* pTerrain = nullptr;
	WaveSurfaceGPU* pWaveSurfaceGPU = nullptr;
	Box* pDisplacementMappingBox = nullptr;
	const DirectX::XMMATRIX ID = DirectX::XMMatrixIdentity();
	Cylinder* pCylinder = nullptr;
	Hills* pPlate = nullptr;
	Box* pBox = nullptr;
	GeoSphere* pGeoSphere = nullptr;
	UINT stride = 0u;
	UINT offset = 0u;
 	Skull* pSkull = nullptr;
	Sky* pSky = nullptr;
  	ShapesDemo shapes;

	float colors[4]{ 0.0392f, 0.0392f, 0.17254f, 1.0f};


};

