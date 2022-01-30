#pragma once
#include <vector>
#include <d3d11.h>
#include "directxmath.h"


//defines the bone transformation at an instant in time
struct Keyframe
{
	Keyframe();
	~Keyframe();

	float timePos;
	DirectX::XMFLOAT3 translation;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT4 rotationQuat;
};


struct BoneAnimation
{
	float GetStartTime() const;
	float GetEndTime() const;

	void Interpolate(float t, DirectX::XMFLOAT4X4& M) const;

	std::vector<Keyframe> keyframes;

};