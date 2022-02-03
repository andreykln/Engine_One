#pragma once
#include <random>
#include <limits>
#include "directxmath.h"
class MathHelper
{
public:
	static float RandomFloatWithinRange(float from, float to);
	static int RandomIntWithingRange(int from, int to);
	static DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX M);
	template<typename T>

	static T Clamp(const T& x, const T& low, const T& high)
	{
		return x < low ? low : (x > high ? high : x);
	}

	template<typename T>
	static T Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}

	template<typename T>
	static T Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	static DirectX::XMVECTOR RandomUnitVec3()
	{
		DirectX::XMVECTOR one = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
		while (true)
		{
			//random points in the cube
			DirectX::XMVECTOR v = DirectX::XMVectorSet(
				RandomFloatWithinRange(-1.0f, 1.0f),
				RandomFloatWithinRange(-1.0f, 1.0f),
				RandomFloatWithinRange(-1.0f, 1.0f), 0.0f);
			//ignore points outside of the unit sphere in order to  
			//get an even distribution over the unit sphere.
			//otherwise points will clump more on the sphere near the corners of the cube
			if (DirectX::XMVector3Greater(DirectX::XMVector3LengthSq(v), one))
			{
				continue;
			}
			return DirectX::XMVector2Normalize(v);
		}

	}
};

