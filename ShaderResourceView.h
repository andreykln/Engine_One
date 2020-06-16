#pragma once
#include "Bindable.h"

class ShaderResourceView : public Bindable
{
public:
	ShaderResourceView(Graphics& gfx, const std::wstring& path);
	void Bind(Graphics& gfx) noexcept override;
private:
	ID3D11Resource* pResource = nullptr;
	ID3D11Texture2D* pTexture = nullptr;
	ID3D11ShaderResourceView* pShaderResourceView = nullptr;
	ID3D11SamplerState* pSamplerState = nullptr;
};

