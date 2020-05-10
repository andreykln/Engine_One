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
};

