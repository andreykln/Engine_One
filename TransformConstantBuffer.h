#pragma once
#include "ConstantBuffer.h"
#include "Shape.h"

class TransformConstantBuffer : public Bindable
{
public:
	TransformConstantBuffer(Graphics& gfx, const Shape& in_parent);
	void Bind(Graphics& gfx) noexcept override;
private:
	VertexConstantBuffer<DirectX::XMMATRIX> pVertexConstBuffer;
	const Shape& parent;

};

