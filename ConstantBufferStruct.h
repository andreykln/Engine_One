#pragma once


// #include "d3d11.h"
class ConstantBufferStruct
{
public:
	ConstantBufferStruct() = default;
// 	ConstantBufferStruct(const ConstantBufferStruct&) = delete;
	virtual ~ConstantBufferStruct() = default;
};

struct EmptyConstBuff : public ConstantBufferStruct
{

};

struct TimeUpdateConstantBuffer : public ConstantBufferStruct
{
	float t;
};