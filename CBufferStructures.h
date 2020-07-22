#pragma once
#include "LightHelper.h"
#include "directxmath.h"

struct Vertex_IA
{
	DirectX::XMFLOAT3 pos;
	const float padding0 = 0.0f;
	DirectX::XMFLOAT3 normal;
	const float padding1 = 0.0f;
	DirectX::XMFLOAT2 tex;
};

struct CBFog
{
	DirectX::XMFLOAT4 fogColor;
	float fogStartandRange[2] = { 0.0f, 0.0f };
	const float padding0[2] = { 0.0f, 0.0f};
};


struct CBPerFrame
{
	CBPerFrame() { ZeroMemory(this, sizeof(this)); }
	DirectionalLight dirLight[3];
	Material objectMaterial;
	DirectX::XMFLOAT3 cbEyePosition;
	int numLights = {3};
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