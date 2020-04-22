#include "Shape.h"
class GeoSphere;
void Shape::AddBind(std::unique_ptr<Bindable> in_bind)
{
	assert("Use AddIndexBuffer for binding IndexBuffer" && typeid(*in_bind) != typeid(IndexBuffer));
	binds.push_back(std::move(in_bind));
}

void Shape::AddIndexBuffer(std::unique_ptr<IndexBuffer> in_bind)
{
	assert("IndexBuffer already bound" && pIndexBuffer == nullptr);
	pIndexBuffer = in_bind.get();
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
