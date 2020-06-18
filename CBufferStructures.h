#pragma once
#include "LightHelper.h"
#include "directxmath.h"
struct CBPerFrame
{
	CBPerFrame() { ZeroMemory(this, sizeof(this)); }
	DirectionalLight dirLight[3];
	DirectX::XMFLOAT3 cbEyePosition;
	int numLights{3};
	Material objectMaterial;
};

struct CBPerObject
{
	CBPerObject() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMMATRIX gWorld;
	DirectX::XMMATRIX gWorldInvTranspose;
	DirectX::XMMATRIX gWorldViewProj;
};
