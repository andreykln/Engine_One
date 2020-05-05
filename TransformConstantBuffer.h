#pragma once
#include "ConstantBuffer.h"
#include "Shape.h"


struct ConstBuf
{
	DirectX::XMMATRIX matrix;
	float time;
};
class TransformConstantBuffer : public Bindable
{
public:
	TransformConstantBuffer(Graphics& gfx, const Shape& in_parent);
// 	TransformConstantBuffer(Graphics& gfx, float in_time, const Shape& in_parent);
	void Bind(Graphics& gfx) noexcept override;
private:
	VertexConstantBuffer<ConstBuf> pVertexConstBuffer;
	const Shape& parent;
	float c_time{};

};

