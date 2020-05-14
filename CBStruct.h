#pragma once
#include "directxmath.h"

struct ConstantBufferParent
{
	ConstantBufferParent() = default;
	~ConstantBufferParent() = default;
};


struct Time_Constant_Buffer : public ConstantBufferParent
{
	DirectX::XMMATRIX matrix;
	float time;
};