#pragma once
#include "Bindable.h"
#include "ConstantBuffer.h"
#include "Shape.h"

class Shape;
class TransformConstantBuffer : public Bindable
{
public:
	TransformConstantBuffer(Graphics& gfx, const Shape& in_parent) noexcept;
	void Bind(Graphics& gfx) noexcept override;
private:
	VertexConstantBuffer<DirectX::XMMATRIX> pVertexConstBuffer;
	const Shape& parent;

};

