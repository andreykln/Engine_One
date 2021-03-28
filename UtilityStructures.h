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



struct TreePointSprite
{
	TreePointSprite() : pos(0.0f, 0.0f, 0.0f), size(0.0f, 0.0f) {};
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 size;
};


struct CBFog
{
	DirectX::XMFLOAT4 fogColor;
	float fogStartandRange[2] = { 5.0f, 105.0f };
	const float padding0[2] = { 0.0f, 0.0f};
};

struct CBPSDepth
{
	DirectX::XMFLOAT4 depthColor;
};

struct CBPerFrame
{
	CBPerFrame() { ZeroMemory(this, sizeof(this)); }
	DirectionalLight dirLight[3];
	Material objectMaterial;
	DirectX::XMFLOAT3 cbEyePosition;
	int numLights = { 3 };
};

struct CBPerFrameTexArray
{
	CBPerFrameTexArray() { ZeroMemory(this, sizeof(this)); }
	DirectionalLight dirLight[3];
	Material objectMaterial;
	DirectX::XMFLOAT3 cbEyePosition;
	int numLights = { 3 };
	int arrayPos[4] = {0,0,0,0}; //padding array, only [0] is used
};

struct CBBillboardGeometry
{
	CBBillboardGeometry() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMMATRIX gWorldViewProj;
	DirectX::XMFLOAT3 cbEyePosition;
	const int padding = 0;
};

struct CBBillboardPixel
{
	CBBillboardPixel() { ZeroMemory(this, sizeof(this)); }
	DirectionalLight dirLight[3];
	Material treeMaterial;
	int numLights[4] = { 3,0,0,0 }; //padding array, only [0] is used
};

struct CBPerFrameMirrorRoom
{
	CBPerFrameMirrorRoom() { ZeroMemory(this, sizeof(this)); }
	DirectionalLight dirLight[3];
};

struct CBPerObject
{
	CBPerObject() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMMATRIX gWorld;
	DirectX::XMMATRIX gWorldInvTranspose;
	DirectX::XMMATRIX gWorldViewProj;
};

struct CBPerObjectTexture
{
	CBPerObjectTexture() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMMATRIX gWorld;
	DirectX::XMMATRIX gWorldInvTranspose;
	DirectX::XMMATRIX gWorldViewProj;
	DirectX::XMMATRIX gTexTransform;
};

enum DemoSwitch
{
	LightningCone,
	Shapesdemo,
	HillsDemo,
	DefaultBox
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
	Keep
};
