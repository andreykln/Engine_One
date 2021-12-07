#include "VertexBufferInstancedDynamic.h"

void VertexBufferInstancedDynamic::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetVertexBuffers(0u, 2u, vBuffers, stride, offset);

}

ID3D11Buffer* VertexBufferInstancedDynamic::Get_p_DynamicInstancedVertexBuffer() const noexcept
{
	return pVertexDynamicInstanced;
}

ID3D11Buffer* VertexBufferInstancedDynamic::GetVertexData()
{
	return pVertexBuffer;
}

ID3D11Buffer* VertexBufferInstancedDynamic::GetInstancedData()
{
	return pVertexDynamicInstanced;
}
