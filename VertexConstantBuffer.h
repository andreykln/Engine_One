#pragma once
#include "Bindable.h"

template <typename C>
class VertexConstantBuffer : public Bindable
{
public:
	VertexConstantBuffer(Graphics& gfx, const C& data)
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
	void Bind(Graphics& gfx) noexcept override
	{
		GetContext(gfx)->VSSetConstantBuffers(0u, 1u, pConstBuffer.GetAddressOf());
	}
	ID3D11Buffer* GetVertexConstantBuffer()
	{
		return pConstBuffer.Get();
	}
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstBuffer;
};

