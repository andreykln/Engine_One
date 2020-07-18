#pragma once
#include "Bindable.h"
class Blending : public Bindable
{
public:
	Blending(Graphics& gfx, D3D11_COLOR_WRITE_ENABLE colorState);
	void Bind(Graphics& gfx) noexcept override;
private:
	float blendFactors[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11BlendState* pBlendState = nullptr;
};

