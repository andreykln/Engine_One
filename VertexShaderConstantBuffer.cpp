#include "VertexShaderConstantBuffer.h"

VertexShaderConstantBuffer::VertexShaderConstantBuffer(Graphics& gfx, const Shape& in_parent, WaveConstantBuffer in_Buffer)
	: buffer(in_Buffer), parent(in_parent), pVertexConstBuffer(gfx)
{
}

void VertexShaderConstantBuffer::Bind(Graphics& gfx) noexcept
{
	parent.SetConstantBuffer();
	pVertexConstBuffer.Update(gfx, buffer);
	pVertexConstBuffer.Bind(gfx);
}
