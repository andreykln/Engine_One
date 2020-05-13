#pragma once
#include "directxmath.h"

class ConstantBufferStruct
{
public:
	ConstantBufferStruct() = default;
	virtual ~ConstantBufferStruct() = default;
};


struct WaveConstantBuffer : public ConstantBufferStruct
{
// 	DirectX::XMMATRIX matrix = DirectX::XMMatrixIdentity();
	float time = 0.0f;
};