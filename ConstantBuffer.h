#pragma once
#include "Bindable.h"

template <typename C>
class ConstantBuffer : public Bindable
{
public:
	void Update(Graphics& gfx, const C& data)
	{
		D3D11_MAPPED_SUBRESOURCE mapped_subresource;
		DX::ThrowIfFailed(GetContext(gfx)->Map(pConstBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mapped_subresource));
		std::memcpy(mapped_subresource.pData, &data, sizeof(data));
		GetContext(gfx)->Unmap(pConstBuffer.Get(), 0u);
	}
	ConstantBuffer(Graphics& gfx, const C& data)
	{
		D3D11_BUFFER_DESC constBufDesc;
		constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constBufDesc.ByteWidth = sizeof(C);
		constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
		constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constBufDesc.StructureByteStride = 0u;
		constBufDesc.MiscFlags = 0u;
		D3D11_SUBRESOURCE_DATA constBufInitData;
		constBufInitData.pSysMem = &data;
		constBufInitData.SysMemPitch = 0;
		constBufInitData.SysMemSlicePitch = 0;
		DX::ThrowIfFailed(GetDevice(gfx)->CreateBuffer(&constBufDesc, &constBufInitData, pConstBuffer.ReleaseAndGetAddressOf()));
	}
	ConstantBuffer(Graphics& gfx)
	{
		D3D11_BUFFER_DESC constBufDesc;
		constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constBufDesc.ByteWidth = sizeof(C);
		constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
		constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constBufDesc.StructureByteStride = 0u;
		constBufDesc.MiscFlags = 0u;
		DX::ThrowIfFailed(GetDevice(gfx)->CreateBuffer(&constBufDesc, nullptr, pConstBuffer.ReleaseAndGetAddressOf()));
	}
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstBuffer;
};

template <typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::ConstantBuffer;
	using ConstantBuffer<C>::pConstBuffer;
public:
	using Bindable::GetContext;
	void Bind(Graphics& gfx) noexcept override
	{
		GetContext(gfx)->VSSetConstantBuffers(0u, 1u, pConstBuffer.GetAddressOf());
	}
};

template <typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstBuffer;
	using ConstantBuffer<C>::ConstantBuffer;
public:
	using Bindable::GetContext;
	void Bind(Graphics& gfx) noexcept override
	{
		GetContext(gfx)->PSSetConstantBuffers(0u, 1u, pConstBuffer.GetAddressOf());
	}
};
