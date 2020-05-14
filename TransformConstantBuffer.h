#pragma once
#include "ConstantBuffer.h"
#include "Shape.h"


struct VS_Constant_Buffer
{
	DirectX::XMMATRIX matrix = DirectX::XMMatrixIdentity();
	float time = 0.0f;
};
class TransformConstantBuffer : public Bindable
{
public:
	TransformConstantBuffer(Graphics& gfx, const Shape& in_parent);
	void Bind(Graphics& gfx) noexcept override;
private:
	VS_Constant_Buffer constBuff;
	BindVertexConstantBuffer<VS_Constant_Buffer> pVertexConstBuffer;
	const Shape& parent;
	float c_time{};

};

