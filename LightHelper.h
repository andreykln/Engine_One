#pragma once
#include "WinBase.h"
#include "directxmath.h"

static DirectX::XMFLOAT3 defaultLightDirection = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
static DirectX::XMFLOAT3 defaultLightStrength = DirectX::XMFLOAT3(0.9f, 0.8f, 0.7f);
struct DirectionalLight
{
	DirectionalLight() { ZeroMemory(this, sizeof(this)); }

	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;
	DirectX::XMFLOAT3 direction;
	float padding{}; 
};

struct PointLight
{
	PointLight() { ZeroMemory(this, sizeof(this)); }

	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;

	//for packing into 4D vector
	DirectX::XMFLOAT3 position;
	float range{};

	//vector (A0, A1, A1, padding)
	DirectX::XMFLOAT3 attenuation; //how light intensity falls of with distance
	float padding{}; //for arrays
};

struct SpotLight
{
	SpotLight() { ZeroMemory(this, sizeof(this)); }

	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;

	//for packing into 4D vector
	DirectX::XMFLOAT3 position;
	float range;

	DirectX::XMFLOAT3 direction;
	float spot;

	//vector (A0, A1, A1, padding)
	DirectX::XMFLOAT3 attenuation; //how light intensity falls of with distance
	float padding{}; //for arrays
};

struct Material
{
	Material() { ZeroMemory(this, sizeof(this)); }

	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular; // w = SpecPower
	DirectX::XMFLOAT4 reflect;
};


struct MaterialEx
{
	DirectX::XMFLOAT4 diffuseAlbedo;
	DirectX::XMFLOAT3 fresnelR0;
	float shininess;
};

struct MaterialM3d
{
	MaterialEx mat;
	std::string name;
	std::wstring diffuseMapName;
	std::wstring normalMapName;
};

struct DirectionalLightEx
{
	DirectionalLightEx() 
		: strength(defaultLightStrength), direction(defaultLightDirection){}

	DirectX::XMFLOAT3 strength; //strength means color of light.
	int pad0;
	DirectX::XMFLOAT3 direction;
	int pad1;
};


/*

struct DirectionalLightEx
{
	DirectionalLightEx()
	{
		ZeroMemory(this, sizeof(this));
	}

	DirectX::XMFLOAT3 strength = defaultLightStrength; //strength means color of light.
	int pad0;
	DirectX::XMFLOAT3 direction;
	int pad1;
};
*/