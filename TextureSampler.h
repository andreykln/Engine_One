#pragma once
#include "Bindable.h"

class TextureSampler : public Bindable
{
public:
	TextureSampler(Graphics& gfx, ShaderType type);
	void Bind(Graphics& gfx) noexcept override;
	ID3D11SamplerState* GetSamplerState();
private:
	ShaderType sType;
	ID3D11SamplerState* pSamplerState = nullptr;
};

