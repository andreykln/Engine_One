#pragma once
#include "WinBase.h"
#include "directxmath.h"

struct DirectionalLight
{
	DirectionalLight() { ZeroMemory(this, sizeof(this)); }

	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;
	DirectX::XMFLOAT3 direction;
	float padding; // for arrays
};

struct PointLight
{
	PointLight() { ZeroMemory(this, sizeof(this)); }

	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;

	//for packing into 4D vector
	DirectX::XMFLOAT3 position;
	float range;

	//vector (A0, A1, A1, padding)
	DirectX::XMFLOAT3 attenuation; //how light intensity falls of with distance
	float padding; //for arrays
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
	float padding; //for arrays
};

struct Material
{
	Material() { ZeroMemory(this, sizeof(this)); }

	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular; // w = SpecPower
	DirectX::XMFLOAT4 reflect;
};