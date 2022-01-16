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


///////// MAIN

struct vbPosNormalTex
{
	vbPosNormalTex() : pos(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), tex(0.0f, 0.0f) {};
	vbPosNormalTex(float posX, float posY, float posZ, float normX, float normY, float normZ, float u, float v)
		: pos(posX, posY, posZ), normal(normX, normY, normZ), tex(u, v) {}
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 tex;
};

struct vbPosNormalTexTangent
{
	vbPosNormalTexTangent() : pos(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), tex(0.0f, 0.0f), tangent(0.0f, 0.0f, 0.0f) {};
	vbPosNormalTexTangent(float posX, float posY, float posZ, float normX, float normY, float normZ, float u, float v,
		float tX, float tY, float tZ)
		: pos(posX, posY, posZ), normal(normX, normY, normZ), tex(u, v), tangent(tX, tY, tZ) {}
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 tex;
	DirectX::XMFLOAT3 tangent;
};

struct cbShadowMap
{
	cbShadowMap() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMMATRIX lightWVP;
	DirectX::XMMATRIX texTransform;
};

struct cbCreateNormalMap
{
	cbCreateNormalMap() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMMATRIX worldView;
	DirectX::XMMATRIX worldInvTransposeView;
	DirectX::XMMATRIX worldViewProjection;
};

struct cbDefaultMatricesVS
{
	cbDefaultMatricesVS() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX viewProjection;
	DirectX::XMMATRIX worldInvTranspose;
	DirectX::XMMATRIX texTransform;
	DirectX::XMMATRIX shadowTransform;
	DirectX::XMMATRIX matTransform;
	DirectX::XMMATRIX worldViewProjTex;
	DirectX::XMFLOAT3 cameraPositon;
	int pad0;
	BOOL enableDisplacementMapping = false;
};

struct cbComputeSSAO
{
	DirectX::XMMATRIX viewToTexSpace;
};

struct cbComputeSSAOconstData
{
	DirectX::XMVECTOR offsetVectors[14];
	DirectX::XMFLOAT4 frustumFarCorners[4];

	const float gOcclusionRadius = 0.5f;
	const float gOcclusionFadeStart = 0.2f;
	const float gOcclusionFadeEnd = 2.0f;
	const float gSurfaceEpsilon = 0.05f;
};

struct cbBlurSSAO
{
	float texelWidth;
	float texelHeight;
	BOOL horizBool;
	int pad0;
};

struct cbDefaultLightPSdata
{
	DirectionalLightEx dirLight;
	DirectX::XMFLOAT4 fogColor = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	DirectX::XMFLOAT4 ambientLight = DirectX::XMFLOAT4(0.25f, 0.25f, 0.35f, 1.0f);
	float fogstart = 50.0f;
	float fogRange = 200.0f;
	int pad2;
	int pad3;

};

struct cbDefaultLightPSPerFrame
{
	MaterialEx mat;
	DirectX::XMFLOAT3 camPositon;
	BOOL disableTexSampling;
	DirectX::XMFLOAT3 lightDirection;
	BOOL useSSAO;
};


struct cbGPUWavesVSConstData
{
	cbGPUWavesVSConstData() { ZeroMemory(this, sizeof(this)); }
	float spatialStep;
	float displacementMapTexelSize[2];
	float padding;
};

struct cbVSTesselationWaves
{
	cbVSTesselationWaves() { ZeroMemory(this, sizeof(this)); }
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

struct CBufferNames
{
	const std::string normalMap = "NormalMap";
	const std::string shadowMap = "ShadowMap";
	const std::string defaultVS = "defaultVS";
	const std::string ssaoBlur = "ssaoBlur";
	const std::string ssaoPerFrame = "ssaoPerFrame";
	//init at runtime
	const std::string ssaoConstData = "ssaoConstData";
	const std::string defaultLightPerFrame = "defaultLightPerFrame";
	const std::string defaultLightData = "defaultLightData";
	const std::string tessWavesMatrices = "tessWavesMatrices";
	const std::string gpuWavesInitData = "GPUWavesVSInitData";
	
};
////////END MAIN

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
	bool useSSAO = true;;
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
struct CB_VS_ShadowMapDrawWithSSAO
{
	CB_VS_ShadowMapDrawWithSSAO() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX worldViewProjection;
	DirectX::XMMATRIX worldInvTranspose;
	DirectX::XMMATRIX texTransform;
	DirectX::XMMATRIX shadowTransform;
	DirectX::XMMATRIX viewProjTex;
};
///////////////////////

struct cbDefaultVS
{
	cbDefaultVS() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX viewProjection;
	DirectX::XMMATRIX worldInvTranspose;
	DirectX::XMMATRIX texTransform;
	DirectX::XMMATRIX shadowTransform;
	DirectX::XMMATRIX matTransform;
	DirectX::XMMATRIX worldViewProjTex[10];
	DirectX::XMFLOAT3 cameraPositon;
	int pad0;
	bool enableDisplacementMapping = false;
	int pad1;
	int pad2;
	int pad3;
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
	BOOL disableTexSampling = false;
	BOOL useSSAO;
	int pad0;
	int pad1;
};


struct cbCreateNormalMapInstanced
{
	cbCreateNormalMapInstanced() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMMATRIX worldView[10];
	DirectX::XMMATRIX worldInvTransposeView[10];
	DirectX::XMMATRIX worldViewProjection[10];
};

enum DemoSwitch
{
	DefaultBox,
	ShadowMap
};


enum ShaderPicker
{
	LightAndTexture_VS_PS,
	Light_VS_PS,
	LightAndTextureArrayPS,
	DepthComplexityVS_PS,
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
	ShadowMapGenSkull_VS_PS,
	ShadowMapDrawSkull_VS_PS,
	DefaultInstanced_PS,
	ShadowMapInstancedGen_VS,
	ShadowMapInstancedDraw_VS,
	ComputeSSAO_VS_PS,
	DrawDebugTexQuad_VS_PS,
	SSAOBlur_VS_PS,
	NormalMap_VS_PS, //new architecture
	ShadowMap_VS_PS, //new architecture
	DefaultLight_VS_PS,
};

enum ParticlePick
{
	Fire,
	Rain,
	Explosion,
	Fountain
};