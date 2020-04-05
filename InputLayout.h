#pragma once
#include "Bindable.h"
class InputLayout : public Bindable
{
public:
	InputLayout(Graphics& gfx, ID3DBlob* pVertexBlobByteCode,
				const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElement,
				const std::wstring& name = std::wstring());
	void Bind(Graphics& gfx) noexcept override;
private:
	Microsoft::WRL::ComPtr< ID3D11InputLayout> pInputLayout;
};

