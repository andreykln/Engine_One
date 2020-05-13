#pragma once
#include "ConstantBuffer.h"
#include "Shape.h"
#include "ConstantBufferStruct.h"

class VertexConstantBuffer : public Bindable
{
public:
	VertexConstantBuffer(Graphics& gfx, const Shape& in_parent);
	void Bind(Graphics& gfx) noexcept override;
private:
//	BindVertexConstantBuffer<ConstantBufferStruct> pVertexConstBuffer;
	BindPixelConstantBuffer<DirectX::XMMATRIX> pVertexConstBuffer;

	const Shape& parent;
};
