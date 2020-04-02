#include "InputLayout.h"

InputLayout::InputLayout(Graphics& gfx, ID3DBlob* pVertexBlobByteCode,const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElement)
{
	DX::ThrowIfFailed(GetDevice(gfx)->CreateInputLayout(
		inputElement.data(),
		static_cast<UINT>(inputElement.size()),
		pVertexBlobByteCode->GetBufferPointer(),
		pVertexBlobByteCode->GetBufferSize(),
		pInputLayout.ReleaseAndGetAddressOf()));
}

void InputLayout::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
}
