#pragma once
#include "Bindable.h"
class IndexBuffer : public Bindable
{
public:
	IndexBuffer(Graphics& gfx, const std::vector<UINT>& indices, const std::wstring& name = std::wstring());
	UINT GetCount() const noexcept;
	void Bind(Graphics& gfx) noexcept override;
private:
	UINT offset{};
	UINT count{};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndicesBuffer;

};

