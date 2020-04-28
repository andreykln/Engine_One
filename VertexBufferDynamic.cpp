#include "VertexBufferDynamic.h"

void VertexBufferDynamic::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexDynamicBuffer.GetAddressOf(), &stride, &offset);


}

ID3D11Buffer* VertexBufferDynamic::Get_p_DynamicVertexBuffer() const noexcept
{
	return pVertexDynamicBuffer.Get();
}
