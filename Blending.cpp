#include "Blending.h"

Blending::Blending(Graphics& gfx, D3D11_COLOR_WRITE_ENABLE colorState)
{

	D3D11_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = colorState;

	DX::ThrowIfFailed(GetDevice(gfx)->CreateBlendState(&blendDesc, &pBlendState));
}

void Blending::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->OMSetBlendState(pBlendState, blendFactors, 0xf00fffff);
}
