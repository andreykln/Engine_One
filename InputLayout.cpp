#include "InputLayout.h"

InputLayout::InputLayout(Graphics& gfx,
						ID3DBlob* pVertexBlobByteCode, 
						const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElement,
						const std::wstring& name)
{
	DX::ThrowIfFailed(GetDevice(gfx)->CreateInputLayout(
		inputElement.data(),
		static_cast<UINT>(inputElement.size()),
		pVertexBlobByteCode->GetBufferPointer(),
		pVertexBlobByteCode->GetBufferSize(),
		pInputLayout.ReleaseAndGetAddressOf()));
#ifdef MY_DEBUG
	if (name != std::wstring())
	{
		gfx.SetDebugName(pInputLayout.Get(), name.c_str());
	}
#endif

}


void InputLayout::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
}
