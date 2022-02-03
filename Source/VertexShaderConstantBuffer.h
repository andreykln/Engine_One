#pragma once
#include "ConstantBuffer.h"
#include "Shape.h"
#include "ConstantBufferStructs.h"

class VertexShaderConstantBuffer : public Bindable
{
public:
	VertexShaderConstantBuffer(Graphics& gfx, const Shape& in_parent, WaveConstantBuffer cBuffer);
	void Bind(Graphics& gfx) noexcept override;
private:
	const Shape& parent;
	ConstantBufferStruct buffer;
	VertexConstantBuffer<ConstantBufferStruct> pVertexConstBuffer;
};


