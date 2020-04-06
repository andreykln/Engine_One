#pragma once
#include "Bindable.h"
class VertexBuffer : public Bindable
{
public:
	template <typename T>
	VertexBuffer(Graphics& gfx, const std::vector<T>& vertices, const std::wstring& name = std::wstring())
		: stride(sizeof(T))
	{ //define it here, so Visual studio won't complain about header file stop
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = static_cast<UINT>(sizeof(T) * vertices.size());//size of the structure
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = sizeof(T); //size of the each element in the structure
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = vertices.data();
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;
		DX::ThrowIfFailed(GetDevice(gfx)->CreateBuffer(&bufferDesc, &initData, pVertexBuffer.ReleaseAndGetAddressOf()));
#ifdef MY_DEBUG
		if (name != std::wstring())
		{
			gfx.SetDebugName(pVertexBuffer.Get(), name.c_str());
		}
#endif
	}
	void Bind(Graphics& gfx) noexcept override;
private:
	UINT offset{};
	UINT stride{};
	Microsoft::WRL::ComPtr <ID3D11Buffer> pVertexBuffer;
};

