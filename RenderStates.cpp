#include "RenderStates.h"
//---------------------------------------------------------------------------------------
// Convenience macro for releasing COM objects.
//---------------------------------------------------------------------------------------

#define ReleaseID3D(x) { if(x){ x->Release(); x = 0; } }

ID3D11RasterizerState* RenderStates::WireframeRS = nullptr;
ID3D11RasterizerState* RenderStates::NoCullRS = nullptr;
ID3D11RasterizerState* RenderStates::CullClockwiseRS = nullptr;
ID3D11RasterizerState* RenderStates::SolidFillRS = nullptr;
ID3D11RasterizerState* RenderStates::CullCounterClockwiseRS = nullptr;

ID3D11BlendState* RenderStates::AlphaToCoverageBS = nullptr;
ID3D11BlendState* RenderStates::TransparentBS = nullptr;
ID3D11BlendState* RenderStates::NoRenderTargetWritesBS = nullptr;
ID3D11BlendState* RenderStates::srsColor = nullptr;

ID3D11DepthStencilState* RenderStates::MarkMirrorDSS = nullptr;
ID3D11DepthStencilState* RenderStates::DrawReflectionDSS = nullptr;
ID3D11DepthStencilState* RenderStates::NoDoubleBlendDSS = nullptr;
ID3D11DepthStencilState* RenderStates::DepthComplexityCountDSS = nullptr;
ID3D11DepthStencilState* RenderStates::DepthComplexityReadDSS = nullptr;




void RenderStates::InitializeAll(Graphics& gfx)
{
	//
	//SolidFIllRS
	//
	D3D11_RASTERIZER_DESC solidFillDesc;
	ZeroMemory(&solidFillDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidFillDesc.FillMode = D3D11_FILL_SOLID;
	solidFillDesc.CullMode = D3D11_CULL_NONE;
	solidFillDesc.FrontCounterClockwise = false;
	solidFillDesc.DepthClipEnable = true;
	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateRasterizerState(&solidFillDesc, &SolidFillRS));


	//
	// WireframeRS
	//
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateRasterizerState(&wireframeDesc, &WireframeRS));

	//
	// NoCullRS
	//
	D3D11_RASTERIZER_DESC noCullDesc;
	ZeroMemory(&noCullDesc, sizeof(D3D11_RASTERIZER_DESC));
	noCullDesc.FillMode = D3D11_FILL_SOLID;
	noCullDesc.CullMode = D3D11_CULL_NONE;
	noCullDesc.FrontCounterClockwise = false;
	noCullDesc.DepthClipEnable = true;

	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateRasterizerState(&noCullDesc, &NoCullRS));

	//
	// CullClockwiseRS
	//

	// Note: Define such that we still cull backfaces by making front faces CCW.
	// If we did not cull backfaces, then we have to worry about the BackFace
	// property in the D3D11_DEPTH_STENCIL_DESC.
	D3D11_RASTERIZER_DESC cullClockwiseDesc;
	ZeroMemory(&cullClockwiseDesc, sizeof(D3D11_RASTERIZER_DESC));
	cullClockwiseDesc.FillMode = D3D11_FILL_SOLID;
	cullClockwiseDesc.CullMode = D3D11_CULL_BACK;
	cullClockwiseDesc.FrontCounterClockwise = true;
	cullClockwiseDesc.DepthClipEnable = true;

	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateRasterizerState(&cullClockwiseDesc, &CullClockwiseRS));

	////Cull counterCloskwise
	D3D11_RASTERIZER_DESC cullCounterClockwiseDesc;
	ZeroMemory(&cullCounterClockwiseDesc, sizeof(D3D11_RASTERIZER_DESC));
	cullCounterClockwiseDesc.FillMode = D3D11_FILL_SOLID;
	cullCounterClockwiseDesc.CullMode = D3D11_CULL_FRONT;
	cullCounterClockwiseDesc.FrontCounterClockwise = true;
	cullCounterClockwiseDesc.DepthClipEnable = true;

	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateRasterizerState(&cullCounterClockwiseDesc, &CullCounterClockwiseRS));


	//
	// AlphaToCoverageBS
	//

	D3D11_BLEND_DESC alphaToCoverageDesc = { 0 };
	alphaToCoverageDesc.AlphaToCoverageEnable = true;
	alphaToCoverageDesc.IndependentBlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].BlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateBlendState(&alphaToCoverageDesc, &AlphaToCoverageBS));

	//
	// TransparentBS
	//

	D3D11_BLEND_DESC transparentDesc = { 0 };
	transparentDesc.AlphaToCoverageEnable = false;
	transparentDesc.IndependentBlendEnable = false;

	transparentDesc.RenderTarget[0].BlendEnable = true;
	transparentDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	transparentDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	transparentDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	transparentDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	transparentDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateBlendState(&transparentDesc, &TransparentBS));

	//colorful one
	D3D11_BLEND_DESC test = { 0 };
	test.AlphaToCoverageEnable = false;
	test.IndependentBlendEnable = false;

	test.RenderTarget[0].BlendEnable = true;
	test.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR;
	test.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	test.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	test.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	test.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	test.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	test.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateBlendState(&test, &srsColor));

	//
	// NoRenderTargetWritesBS
	//

	D3D11_BLEND_DESC noRenderTargetWritesDesc = { 0 };
	noRenderTargetWritesDesc.AlphaToCoverageEnable = false;
	noRenderTargetWritesDesc.IndependentBlendEnable = false;

	noRenderTargetWritesDesc.RenderTarget[0].BlendEnable = false;
	noRenderTargetWritesDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	noRenderTargetWritesDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	noRenderTargetWritesDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	noRenderTargetWritesDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	noRenderTargetWritesDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	noRenderTargetWritesDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	noRenderTargetWritesDesc.RenderTarget[0].RenderTargetWriteMask = 0;

	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateBlendState(&noRenderTargetWritesDesc, &NoRenderTargetWritesBS));

	//
	// MarkMirrorDSS
	//

	D3D11_DEPTH_STENCIL_DESC mirrorDesc;
	mirrorDesc.DepthEnable = true;
	mirrorDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	mirrorDesc.DepthFunc = D3D11_COMPARISON_LESS; 
	mirrorDesc.StencilEnable = true;
	mirrorDesc.StencilReadMask = 0xff;
	mirrorDesc.StencilWriteMask = 0xff;

	mirrorDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	mirrorDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// We are not rendering backfacing polygons, so these settings do not matter.
	mirrorDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	mirrorDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateDepthStencilState(&mirrorDesc, &MarkMirrorDSS));

	//
	// DrawReflectionDSS
	//

	D3D11_DEPTH_STENCIL_DESC drawReflectionDesc;
	drawReflectionDesc.DepthEnable = true;
	drawReflectionDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	drawReflectionDesc.DepthFunc = D3D11_COMPARISON_LESS;
	drawReflectionDesc.StencilEnable = true; 
	drawReflectionDesc.StencilReadMask = 0xff;
	drawReflectionDesc.StencilWriteMask = 0xff;

	drawReflectionDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	// We are not rendering backfacing polygons, so these settings do not matter.
	drawReflectionDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateDepthStencilState(&drawReflectionDesc, &DrawReflectionDSS));

	//
	// NoDoubleBlendDSS
	//

	D3D11_DEPTH_STENCIL_DESC noDoubleBlendDesc;
	noDoubleBlendDesc.DepthEnable = true;
	noDoubleBlendDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	noDoubleBlendDesc.DepthFunc = D3D11_COMPARISON_LESS;
	noDoubleBlendDesc.StencilEnable = true; 
	noDoubleBlendDesc.StencilReadMask = 0xff;
	noDoubleBlendDesc.StencilWriteMask = 0xff;

	noDoubleBlendDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	noDoubleBlendDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	noDoubleBlendDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	noDoubleBlendDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	// We are not rendering backfacing polygons, so these settings do not matter.
	noDoubleBlendDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	noDoubleBlendDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	noDoubleBlendDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	noDoubleBlendDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateDepthStencilState(&noDoubleBlendDesc, &NoDoubleBlendDSS));

	//
	//DepthComplexityCounter
	//
	D3D11_DEPTH_STENCIL_DESC depthComplCountDesc;
	depthComplCountDesc.DepthEnable = true;
	depthComplCountDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthComplCountDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	depthComplCountDesc.StencilEnable = true;
	depthComplCountDesc.StencilReadMask = 0xff;
	depthComplCountDesc.StencilWriteMask = 0xff;

	depthComplCountDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INCR;
	depthComplCountDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthComplCountDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	depthComplCountDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// We are not rendering backfacing polygons, so these settings do not matter.
	depthComplCountDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthComplCountDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthComplCountDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	depthComplCountDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;


	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateDepthStencilState(&depthComplCountDesc, &DepthComplexityCountDSS));

	//
	//DepthComplexityRead
	//
	D3D11_DEPTH_STENCIL_DESC depthComplReadDesc;
	depthComplReadDesc.DepthEnable = true;
	depthComplReadDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthComplReadDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	depthComplReadDesc.StencilEnable = true;
	depthComplReadDesc.StencilReadMask = 0xff;
	depthComplReadDesc.StencilWriteMask = 0xff;

	depthComplReadDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthComplReadDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthComplReadDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthComplReadDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	// We are not rendering backfacing polygons, so these settings do not matter.
	depthComplReadDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthComplReadDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthComplReadDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	depthComplReadDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;


	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateDepthStencilState(&depthComplReadDesc, &DepthComplexityReadDSS));


}

void RenderStates::DestroyAll()
{
	ReleaseID3D(WireframeRS);
	ReleaseID3D(NoCullRS);
	ReleaseID3D(CullClockwiseRS);
	ReleaseID3D(CullCounterClockwiseRS);
	ReleaseID3D(SolidFillRS);

	ReleaseID3D(AlphaToCoverageBS);
	ReleaseID3D(TransparentBS);
	ReleaseID3D(NoRenderTargetWritesBS);

	ReleaseID3D(MarkMirrorDSS);
	ReleaseID3D(DrawReflectionDSS);
	ReleaseID3D(NoDoubleBlendDSS);
	ReleaseID3D(DepthComplexityCountDSS);
	ReleaseID3D(DepthComplexityReadDSS);

}

void RenderStates::Bind(Graphics& gfx) noexcept
{

}
