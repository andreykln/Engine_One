#pragma once
#include <d3d11.h>

struct RasterizerState
{
	D3D11_RASTERIZER_DESC Wireframe()
	{
		rasterDesc.AntialiasedLineEnable = FALSE;
		rasterDesc.CullMode = D3D11_CULL_BACK; //D3D11_CULL_BACK
		rasterDesc.FrontCounterClockwise = FALSE;
		rasterDesc.DepthBias = 0u;
		rasterDesc.SlopeScaledDepthBias = 0.0f;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
		rasterDesc.DepthClipEnable = TRUE;
		rasterDesc.ScissorEnable = FALSE;
		rasterDesc.MultisampleEnable = FALSE;
		return rasterDesc;
	}
	D3D11_RASTERIZER_DESC SolidFill()
	{
		rasterDesc.AntialiasedLineEnable = FALSE;
		rasterDesc.CullMode = D3D11_CULL_BACK; 
		rasterDesc.FrontCounterClockwise = FALSE;
		rasterDesc.DepthBias = 0u;
		rasterDesc.SlopeScaledDepthBias = 0.0f;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.DepthClipEnable = TRUE;
		rasterDesc.ScissorEnable = FALSE;
		rasterDesc.MultisampleEnable = FALSE;
		return rasterDesc;
	}


private:
	D3D11_RASTERIZER_DESC rasterDesc;

};
