#include "TransformConstantBuffer.h"

TransformConstantBuffer::TransformConstantBuffer(Graphics& gfx, const Shape& in_parent)
	: parent(in_parent), pVertexConstBuffer(gfx)
{

}

// TransformConstantBuffer::TransformConstantBuffer(Graphics& gfx, float in_time, const Shape& in_parent)
// 	: parent(in_parent), pVertexConstBuffer(gfx), c_time(in_time)
// {
// 
// }

void TransformConstantBuffer::Bind(Graphics& gfx) noexcept
{
// 	struct ConstBuf
// 	{
// 		DirectX::XMMATRIX matrix;
// 
// 	};

	ConstBuf test;
	test.time = parent.GetAlpha();
	test.matrix = DirectX::XMMatrixTranspose(parent.GetTransform() * gfx.GetProjection());
	pVertexConstBuffer.Update(gfx, test);


// 	pVertexConstBuffer.Update(gfx, DirectX::XMMatrixTranspose(parent.GetTransform() * gfx.GetProjection()));
	pVertexConstBuffer.Bind(gfx);
}
