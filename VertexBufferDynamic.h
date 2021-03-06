#pragma once
#include "Bindable.h"
class VertexBufferDynamic : public Bindable
{
public:
	template <typename T>
	VertexBufferDynamic(Graphics& gfx, const std::vector<T>& vertices, const std::wstring& name = std::wstring())
		: stride(sizeof(T))
	{ //define it here, so Visual studio won't complain about header file stop
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = static_cast<UINT>(sizeof(T) * vertices.size());//size of the structure
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = sizeof(T); //size of the each element in the structure
		DX::ThrowIfFailed(GetDevice(gfx)->CreateBuffer(&bufferDesc, 0u, pVertexDynamicBuffer.ReleaseAndGetAddressOf()));
#ifdef MY_DEBUG
		if (name != std::wstring())
		{
			gfx.SetDebugName(pVertexDynamicBuffer.Get(), name.c_str());
		}
#endif
	}

	void Bind(Graphics& gfx) noexcept override;
	ID3D11Buffer* Get_p_DynamicVertexBuffer() const noexcept;
private:
	UINT offset{};
	UINT stride{};
	Microsoft::WRL::ComPtr <ID3D11Buffer> pVertexDynamicBuffer;
};

