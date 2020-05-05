#include "TransformConstantBuffer.h"

TransformConstantBuffer::TransformConstantBuffer(Graphics& gfx, const Shape& in_parent)
	: parent(in_parent), pVertexConstBuffer(gfx)
{

}

void TransformConstantBuffer::Bind(Graphics& gfx) noexcept
{
	constBuff.time = parent.GetAlpha();
	constBuff.matrix = DirectX::XMMatrixTranspose(parent.GetTransform() * gfx.GetProjection());

	pVertexConstBuffer.Update(gfx, constBuff);
	pVertexConstBuffer.Bind(gfx);
}
