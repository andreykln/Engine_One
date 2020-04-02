#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices)
	: count(static_cast<UINT>(indices.size()))
{
	D3D11_BUFFER_DESC indicesBuffDesc;
	indicesBuffDesc.Usage = D3D11_USAGE_DEFAULT;
	indicesBuffDesc.ByteWidth = static_cast<UINT>(sizeof(unsigned short) * count);
	indicesBuffDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indicesBuffDesc.CPUAccessFlags = 0u;
	indicesBuffDesc.MiscFlags = 0u;
	indicesBuffDesc.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA indicesInitData;
	indicesInitData.pSysMem = indices.data();
	indicesInitData.SysMemPitch = 0u;
	indicesInitData.SysMemSlicePitch = 0u;
	DX::ThrowIfFailed(GetDevice(gfx)->CreateBuffer(&indicesBuffDesc, &indicesInitData, pIndicesBuffer.ReleaseAndGetAddressOf()));
}

UINT IndexBuffer::GetCount() const noexcept
{
	return count;
}

void IndexBuffer::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetIndexBuffer(pIndicesBuffer.Get(), DXGI_FORMAT_R16_UINT, offset);
}
