#pragma once
#include "Bindable.h"

template <typename C>
class PixelShaderConstantBuffer : public Bindable
{
public:
	PixelShaderConstantBuffer(Graphics& gfx, const C& data, UINT in_startSlot, UINT in_numBuffers,
		D3D11_CPU_ACCESS_FLAG in_cpuFlag, D3D11_USAGE in_usageFlag)
		: numBuffers(in_numBuffers), startSlot(in_startSlot), cpuFlag(in_cpuFlag), usageFlag(in_usageFlag)
	{
		D3D11_BUFFER_DESC constBufDesc;
		constBufDesc.CPUAccessFlags = cpuFlag;
		constBufDesc.ByteWidth = sizeof(C);
		constBufDesc.Usage = usageFlag;
		constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constBufDesc.StructureByteStride = 0u;
		constBufDesc.MiscFlags = 0u;
		D3D11_SUBRESOURCE_DATA constBufInitData;
		constBufInitData.pSysMem = &data;
		constBufInitData.SysMemPitch = 0;
		constBufInitData.SysMemSlicePitch = 0;
		DX::ThrowIfFailed(GetDevice(gfx)->CreateBuffer(&constBufDesc, &constBufInitData,
			pConstBuffer.ReleaseAndGetAddressOf()));
	}

	void Bind(Graphics& gfx) noexcept override
	{
		GetContext(gfx)->PSSetConstantBuffers(startSlot, numBuffers, pConstBuffer.GetAddressOf());
	}
	ID3D11Buffer* GetPixelShaderConstantBuffer()
	{
		return pConstBuffer.Get();
	}
private:
	D3D11_CPU_ACCESS_FLAG cpuFlag;
	D3D11_USAGE usageFlag;
	UINT numBuffers{ 1 };
	UINT startSlot{ 0 };
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstBuffer;
};

