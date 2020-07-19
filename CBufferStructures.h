#pragma once
#include "LightHelper.h"
#include "directxmath.h"



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