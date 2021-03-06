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
};

