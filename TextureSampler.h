#pragma once
#include "Bindable.h"

class TextureSampler : public Bindable
{
public:
	TextureSampler(Graphics& gfx);
	void Bind(Graphics& gfx) noexcept override;

private:
	ID3D11SamplerState* pSamplerState = nullptr;
};

