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

static std::vector<D3D11_INPUT_ELEMENT_DESC> inputLightTexture =
{
	{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u,
	D3D11_INPUT_PER_VERTEX_DATA, 0u},
	{"Normal", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, sizeof(DirectX::XMFLOAT3),
	D3D11_INPUT_PER_VERTEX_DATA, 0u},
	{"TexCoordinate", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, sizeof(DirectX::XMFLOAT3) * 2,
	D3D11_INPUT_PER_VERTEX_DATA, 0u}
};


struct CBFog
{
	DirectX::XMFLOAT4 fogColor;
	float fogStartandRange[2] = { 5.0f, 105.0f };
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

struct CBPerFrameMirrorRoom
{
	CBPerFrameMirrorRoom() { ZeroMemory(this, sizeof(this)); }
	DirectionalLight dirLight[3];
	Material objectMaterial;
	DirectX::XMFLOAT3 cbEyePosition;
	int numLights = { 3 };
	int currentTexture[4];
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