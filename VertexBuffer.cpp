#include "VertexBuffer.h"

void VertexBuffer::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

}

ID3D11Buffer* VertexBuffer::Get_p_VertexBuffer() const noexcept
{
	return pVertexBuffer.Get();
}
