#include "TransformConstantBuffer.h"

TransformConstantBuffer::TransformConstantBuffer(Graphics& gfx, const Shape& in_parent)
	: parent(in_parent), pVertexConstBuffer(gfx)
{
}

void TransformConstantBuffer::Bind(Graphics& gfx) noexcept
{
	pVertexConstBuffer.Update(gfx, DirectX::XMMatrixTranspose(parent.GetTransform() * gfx.GetProjection()));
	pVertexConstBuffer.Bind(gfx);
}
