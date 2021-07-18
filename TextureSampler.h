#pragma once
#include "Bindable.h"

class TextureSampler : public Bindable
{
public:
	TextureSampler(Graphics& gfx, ShaderType type);
	void Bind(Graphics& gfx) noexcept override;

private:
	ShaderType sType;
	ID3D11SamplerState* pSamplerState = nullptr;
};

