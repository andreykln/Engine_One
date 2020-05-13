#include "VertexConstantBuffer.h"

VertexConstantBuffer::VertexConstantBuffer(Graphics& gfx, const Shape& in_parent)
	: parent(in_parent), pVertexConstBuffer(gfx)
{

}

void VertexConstantBuffer::Bind(Graphics& gfx) noexcept
{
	pVertexConstBuffer.Update(gfx, parent.ReturnXMMatrixForTest());
	pVertexConstBuffer.Bind(gfx);
}
