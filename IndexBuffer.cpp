#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<UINT>& indices, const std::wstring& name)
	: count(static_cast<UINT>(indices.size()))
{
	D3D11_BUFFER_DESC indicesBuffDesc;
	indicesBuffDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indicesBuffDesc.ByteWidth = (sizeof(UINT) * count);
	indicesBuffDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indicesBuffDesc.CPUAccessFlags = 0u;
	indicesBuffDesc.MiscFlags = 0u;
	indicesBuffDesc.StructureByteStride = sizeof(UINT);
	D3D11_SUBRESOURCE_DATA indicesInitData;
	indicesInitData.pSysMem = indices.data();
	indicesInitData.SysMemPitch = 0u;
	indicesInitData.SysMemSlicePitch = 0u;
	DX::ThrowIfFailed(GetDevice(gfx)->CreateBuffer(&indicesBuffDesc, &indicesInitData, pIndicesBuffer.ReleaseAndGetAddressOf()));
#ifdef MY_DEBUG
	if (name != std::wstring())
	{
		gfx.SetDebugName(pIndicesBuffer.Get(), name.c_str());
	}
#endif
}

UINT IndexBuffer::GetCount() const noexcept
{
	return count;
}

void IndexBuffer::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetIndexBuffer(pIndicesBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);
}
