#include "Rasterizer.h"

Rasterizer::Rasterizer(Graphics& gfx, D3D11_RASTERIZER_DESC desc)
{
	DX::ThrowIfFailed(GetDevice(gfx)->CreateRasterizerState(&desc, pRasterState.ReleaseAndGetAddressOf()));
}

void Rasterizer::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->RSSetState(pRasterState.Get());
}
