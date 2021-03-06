#include "MathHelper.h"

float MathHelper::RandomFloatWithinRange(float from, float to)
{
	std::random_device device;
	std::mt19937 gen(device());
	std::uniform_real_distribution<float> dis(from, to);
	return dis(gen);
}

int MathHelper::RandomIntWithingRange(int from, int to)
{
	std::random_device device;
	std::mt19937 gen(device());
	std::uniform_int_distribution<int> dis(from, to);
	return dis(gen);
}

DirectX::XMMATRIX MathHelper::InverseTranspose(DirectX::CXMMATRIX M)
{
	DirectX::XMMATRIX A = M;
	A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(A);
	auto a = (DirectX::XMMatrixInverse(&det, A));
	return/* DirectX::XMMatrixTranspose*/(DirectX::XMMatrixInverse(&det, A));
}

