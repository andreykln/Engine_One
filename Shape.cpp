#include "Shape.h"
class GeoSphere;

void Shape::Update(float dt) noexcept
{
	alpha = dt;
}

void Shape::AddBind(Bindable* in_bind)
{
	assert("Use AddIndexBuffer for binding IndexBuffer" && typeid(*in_bind) != typeid(IndexBuffer));
	binds.push_back(std::move(in_bind));
}

void Shape::AddIndexBuffer(IndexBuffer* in_bind)
{
	assert("IndexBuffer already bound" && pIndexBuffer == nullptr);
	pIndexBuffer = in_bind;
	binds.push_back(std::move(in_bind));
}

void Shape::BindAndDraw(Graphics& gfx) const noexcept
{
	for (auto& b : binds)
	{
		b->Bind(gfx);
	}
	gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Shape::SetMatrix(DirectX::XMMATRIX in_matrix) noexcept
{
	m_Matrix = in_matrix;
}
