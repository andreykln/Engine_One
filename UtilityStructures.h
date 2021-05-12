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

struct CB_VS_Transform
{
	CB_VS_Transform() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX worldViewProjection;
	DirectX::XMMATRIX worldInvTranspose;
	DirectX::XMMATRIX texTransform;
};

struct CB_PS_DirectionalL_Fog
{
	CB_PS_DirectionalL_Fog() { ZeroMemory(this, sizeof(this)); }
	DirectionalLight dirLight[3];
	Material mat;
	DirectX::XMFLOAT4 fogColor = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	float fogstart = 50.0f;
	float fogRange = 200.0f;
	float padding[2];
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

///hackjob
struct CB_PS_MirrorRoom
{
	CB_PS_MirrorRoom() { ZeroMemory(this, sizeof(this)); }
	DirectionalLight dirLight[3];
	DirectX::XMFLOAT3 cameraPosition;
	unsigned int numberOfLights = 2u;
};

struct CB_PS_Skull_Mirror
{
	CB_PS_Skull_Mirror() { ZeroMemory(this, sizeof(this)); }
	DirectionalLight dirLight[3];
	DirectX::XMFLOAT3 cameraPosition;
	unsigned int numberOfLights = 2u;
};

struct CB_PS_Skull_Mat
{
	Material mat;
};
//hackjob ends
struct TreePointSprite
{
	TreePointSprite() : pos(0.0f, 0.0f, 0.0f), size(0.0f, 0.0f) {};
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 size;
};


// struct CBFog
// {
// 	DirectX::XMFLOAT4 fogColor;
// 	float fogStartandRange[2] = { 5.0f, 105.0f };
// 	const float padding0[2] = { 0.0f, 0.0f};
// };
// 
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


enum DemoSwitch
{
	LightningCone,
	Shapesdemo,
	HillsDemo,
	HillsAllLight,
	DefaultBox,
	MirrorSkull,
};


enum ShaderPicker
{
	LightAndTexture_VS_PS,
	Light_VS_PS,
	MirrorRoomPS,
	LightAndTextureArrayPS,
	DepthComplexityVS_PS,
	TreeBillboardVS_PS_GS,
	CircleToCylinderVS_GS_PS,
	MirrorSkull_PS,
	HillsAllLight_PS,
	HorizontalBlur_CS,
	BlurTexture_PS,
	VerticalBlur_CS,
	VerticalBilateralBlur_CS,
	HorizontalBilateralBlur_CS,
	UpdateWaves_CS,
	DisturbWaves_CS,
	GPUWaves_VS,
	Keep
};
