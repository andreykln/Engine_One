#pragma once
#include "Bindable.h"
class PixelShader : public Bindable
{
public:
	PixelShader(Graphics& gfx, const std::wstring& path);
	void Bind(Graphics& gfx) noexcept override;
private:
	UINT numOfClassInstances{};
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
};

