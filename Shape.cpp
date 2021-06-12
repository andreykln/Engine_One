#include "Shape.h"



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

void Shape::BindAndDrawIndexed(Graphics& gfx) const noexcept
{
	for (auto& b : binds)
	{
		b->Bind(gfx);
	}
	gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Shape::BindAndDrawIndexed(Graphics& gfx, UINT count, UINT startIndexLocation, UINT startVertexLocation)
{
	for (auto& b : binds)
	{
		b->Bind(gfx);
	}
	gfx.DrawIndexed(count, startIndexLocation, startVertexLocation);
}

void Shape::BindAndDraw(Graphics& gfx, UINT VertexCount, UINT StartVertexLocation) const noexcept
{
	for (auto& b : binds)
	{
		b->Bind(gfx);
	}
	gfx.Draw(VertexCount, StartVertexLocation);
}

void Shape::BindAndDrawSeveral(Graphics& gfx, UINT count, UINT startIndexLocation, INT baseVertexLocation) noexcept
{
	for (auto& b : binds)
	{
		b->Bind(gfx);
	}
	gfx.DrawIndexedTwo(count, startIndexLocation, baseVertexLocation);

}

void Shape::BindAndDrawInstancedIndexed(Graphics& gfx, UINT instanceCount, UINT startIndexLocation, int baseVertexLocation, UINT startInstanceLocation)
{
	for (auto& b : binds)
	{
		b->Bind(gfx);
	}
	gfx.DrawInstancedIndexed(pIndexBuffer->GetCount(), instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);

}

void Shape::DeleteObject() const noexcept
{
	for (auto& b : binds)
	{
		delete b;
	}
}


