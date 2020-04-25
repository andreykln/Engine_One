#include "MathHelper.h"

float MathHelper::RandomFloatWithinRange(float from, float to)
{
	static std::random_device device;
	static std::mt19937 gen(device());
	static std::uniform_real_distribution<float> dis(from, to);
	return dis(gen);
}

int MathHelper::RandomInt()
{
	static std::random_device device;
	static std::mt19937 gen(device());
	static std::uniform_int_distribution<int> dis(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
	return dis(gen);
}

