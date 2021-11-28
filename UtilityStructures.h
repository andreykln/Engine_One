#pragma once
#include "LightHelper.h"
#include "directxmath.h"
struct Vertex_IA
{
	Vertex_IA() : pos(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), tex(0.0f, 0.0f) {};
	Vertex_IA(float posX, float posY, float posZ, float normX, float normY, float normZ, float u, float v)
		: pos(posX, posY, posZ), normal(normX, normY, normZ), tex(u, v) {}
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 tex;
};

struct Vertices_Full
{
	Vertices_Full() : pos(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), tex(0.0f, 0.0f), tangent(0.0f, 0.0f, 0.0f) {};
	Vertices_Full(float posX, float posY, float posZ, float normX, float normY, float normZ, float u, float v,
		float tX, float tY, float tZ)
		: pos(posX, posY, posZ), normal(normX, normY, normZ), tex(u, v), tangent(tX, tY, tZ) {}
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 tex;
	DirectX::XMFLOAT3 tangent;
};

struct CB_VS_Transform
{
	CB_VS_Transform() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX worldViewProjection;
	DirectX::XMMATRIX worldInvTranspose;
	DirectX::XMMATRIX texTransform;
};

struct CB_VS_DisplacementWavesPerFrame
{
	CB_VS_DisplacementWavesPerFrame() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX worldViewProjection;
	DirectX::XMMATRIX worldInvTranspose;
	DirectX::XMMATRIX texTransform;
	DirectX::XMMATRIX waveDispTexTransform0;
	DirectX::XMMATRIX waveDispTexTransform1;
	DirectX::XMMATRIX waveNormalTexTransform0;
	DirectX::XMMATRIX waveNormalTexTransform1;
	DirectX::XMFLOAT3 cameraPosition;
};

struct CB_VS_TransformWithCameraPosition
{
	CB_VS_TransformWithCameraPosition() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX worldViewProjection;
	DirectX::XMMATRIX worldInvTranspose;
	DirectX::XMMATRIX texTransform;
	DirectX::XMFLOAT3 cameraPosition;
};

struct CB_PS_NormalMappingState
{
	bool normalMapping;
	int padding[3] = { 0,0,0 };
};

struct CB_HS_TerrainPerFrame
{
	CB_HS_TerrainPerFrame() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMFLOAT4 worldFrustumPlanes[6];
	DirectX::XMFLOAT3 cameraPosition;
	float padding = 0.0f;
	

};

struct CB_PS_Terrain
{
	CB_PS_Terrain() { ZeroMemory(this, sizeof(this)); }
	float texelCellSpaceU;
	float texelCellSpaceV;
	float worldCellSpace;
	float padding;
};

struct CB_VS_WorldViewProjection
{
	CB_VS_WorldViewProjection() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMMATRIX worldViewProjection;
	DirectX::XMMATRIX world;
};

struct CB_CameraPosition_ViewProj
{
	CB_CameraPosition_ViewProj() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMMATRIX viewProjection;
	DirectX::XMFLOAT3 cameraPosition;
};

struct CB_PS_DirectionalL_Fog
{
	CB_PS_DirectionalL_Fog() { ZeroMemory(this, sizeof(this)); }
	DirectionalLight dirLight[3];
	Material mat;
	DirectX::XMFLOAT4 fogColor = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	const float fogstart = 50.0f;
	const float fogRange = 200.0f;
	float padding[2];
};

struct CB_PS_DirectionalEX_Fog
{
	CB_PS_DirectionalEX_Fog() { ZeroMemory(this, sizeof(this)); }
	DirectionalLightEx dirLight;
	MaterialEx mat;
	DirectX::XMFLOAT4 ambientLight = DirectX::XMFLOAT4(0.25f, 0.25f, 0.35f, 1.0f);
	DirectX::XMFLOAT4 fogColor = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	const float fogstart = 50.0f;
	const float fogRange = 200.0f;
	float pad1;
	float pad2;
};

struct CB_PS_Dir_Point_Spot_Fog_Lights
{
	CB_PS_Dir_Point_Spot_Fog_Lights() { ZeroMemory(this, sizeof(this)); }
	DirectionalLight dirLight[3];
	PointLight pointLight;
	SpotLight spotLight;
	Material mat;
	DirectX::XMFLOAT3 cameraPosition;
	unsigned int numberOfLights = 1u;
	DirectX::XMFLOAT4 fogColor = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	float fogstart = 50.0f;
	float fogRange = 200.0f;
	float padding[2];
};

struct CB_PS_PerFrameUpdate
{
	CB_PS_PerFrameUpdate() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMFLOAT3 cameraPositon;
	unsigned int numberOfLights = 1u;
	unsigned int texArrayPos = 0u;
	float padding0;
	float padding1;
	float padding2;

};

struct CB_CS_GPUWaves
{
	CB_CS_GPUWaves() { ZeroMemory(this, sizeof(this)); }
	float waveConstant0;
	float waveConstant1;
	float waveConstant2;
	float disturbMagnitute;

	int disturbIndex[2];
	int padding[2];
};

struct CB_VS_GPUWaves_consts
{
	CB_VS_GPUWaves_consts() { ZeroMemory(this, sizeof(this)); }
	float spatialStep;
	float displacementMapTexelSize[2];
	float padding;
};

struct TreePointSprite
{
	TreePointSprite() : pos(0.0f, 0.0f, 0.0f), size(0.0f, 0.0f) {};
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 size;
};

struct CBPSDepth
{
	DirectX::XMFLOAT4 depthColor;
};

struct CBBillboardGeometry
{
	CBBillboardGeometry() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMMATRIX worldViewProjection;
	DirectX::XMFLOAT3 cameraPosition;
	const int padding = 0;
};

struct CBBillboardPixel
{
	CBBillboardPixel() { ZeroMemory(this, sizeof(this)); }
	DirectionalLight dirLight[3];
	Material treeMaterial;
	int numLights[4] = { 3,0,0,0 }; //padding array, only [0] is used
};

struct CB_QuadTess_DS_WVP
{
	CB_QuadTess_DS_WVP() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMMATRIX worldViewProjection;
};

struct CB_QuadTess_HS
{
	CB_QuadTess_HS() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMMATRIX world;
	DirectX::XMFLOAT3 cameraPosition;
};

struct CB_GS_StreamOut
{
	CB_GS_StreamOut() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMFLOAT3  emitterPositon;
	float timeStep;
	float gameTime;
	float padding[3] = {0.0f, 0.0f, 0.0f};
};

struct Particle
{
	DirectX::XMFLOAT3 initialPos;
	DirectX::XMFLOAT3 initialVel;
	DirectX::XMFLOAT2 size;
	float age;
	unsigned int type;
};

//
// shadow map
//
struct ShadowMapGenVS
{
	ShadowMapGenVS() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMMATRIX lightWVP;
	DirectX::XMMATRIX texTransform;
};
struct CB_PS_ShadowMapDraw
{
	CB_PS_ShadowMapDraw() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMFLOAT3 lightDirection;
	unsigned int numberOfLights = 1;
	DirectX::XMFLOAT3 cameraPositon;
	int pad0;
};
struct CB_VS_ShadowMapDraw
{
	CB_VS_ShadowMapDraw() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX worldViewProjection;
	DirectX::XMMATRIX worldInvTranspose;
	DirectX::XMMATRIX texTransform;
	DirectX::XMMATRIX shadowTransform;
};
///////////////////////

struct cbDefaultVS
{
	cbDefaultVS() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX worldViewProjection;
	DirectX::XMMATRIX worldInvTranspose;
	DirectX::XMMATRIX texTransform;
	DirectX::XMMATRIX shadowTransform;
	DirectX::XMMATRIX matTransform;
};

struct cbDefaultPS
{
	cbDefaultPS() { ZeroMemory(this, sizeof(this)); }
	DirectionalLightEx dirLight;
	MaterialEx mat;
	DirectX::XMFLOAT4 fogColor = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	DirectX::XMFLOAT4 ambientLight = DirectX::XMFLOAT4(0.25f, 0.25f, 0.35f, 1.0f);
	DirectX::XMFLOAT3 lightDirection;
	const float fogstart = 50.0f;
	DirectX::XMFLOAT3 camPositon;
	const float fogRange = 200.0f;
	bool enableNormalMapping = false;
	int pad0;
	int pad1;
	int pad2;
};

enum DemoSwitch
{
	LightningCone,
	Shapesdemo,
	HillsDemo,
	HillsAllLight,
	DefaultBox,
	MirrorSkull,
	DisplacementMapping,
	ShadowMap
};


enum ShaderPicker
{
	LightAndTexture_VS_PS,
	Light_VS_PS,
	LightAndTextureArrayPS,
	DepthComplexityVS_PS,
	TreeBillboardVS_PS_GS,
	CircleToCylinderVS_GS_PS,
	HillsAllLight_PS,
	HorizontalBlur_CS,
	BlurTexture_PS,
	VerticalBlur_CS,
	VerticalBilateralBlur_CS,
	HorizontalBilateralBlur_CS,
	UpdateWaves_CS,
	DisturbWaves_CS,
	GPUWaves_VS,
	QuadTessellation_VS,
	QuadTessellation_PS,
	QuadTessellation_DS,
	QuadTessellation_HS,
	InstancedSkull_VS,
	InstancedSkull_PS,
	Sky_VS_PS,
	CubeMap_PS,
	LightAndTextureNormalMapping_VS_PS,
	DisplacementMapping_VS_DS_HS,
	DisplacementWaves_VS_HS_DS_PS,
	TerrainHeightMap_VS_PS_DS_HS_PS,
	Particles_FireStreamOut_VS_GS,
	Particles_FireDraw_VS_GS_PS,
	Particles_RainStreamOut_VS_GS,
	Particles_RainDraw_VS_GS_PS,
	Particles_ExplosionStreamOut_VS_GS,
	Particle_ExplosionDraw_VS_GS_PS,
	Particle_FountainStreamOut_VS_GS,
	Particle_FountainDraw_VS_GS_PS,
	ShadowMap_VS_PS,
	ShadowMapGen_VS_PS,
	ShadowMapGenSkull_VS_PS,
	ShadowMapDrawSkull_VS_PS,
	Keep
};

enum ParticlePick
{
	Fire,
	Rain,
	Explosion,
	Fountain
};