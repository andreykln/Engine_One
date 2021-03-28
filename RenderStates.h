#pragma once
#include "d3d11.h"
#include "Bindable.h"
// #include "Graphics.h"
//not actually a bindable class, it inherits for convenient access to the device
class RenderStates : public Bindable
{
public:
	static void InitializeAll(Graphics& gfx);
	static void DestroyAll();

	//Rasterizer states
	static ID3D11RasterizerState* WireframeRS;
	static ID3D11RasterizerState* NoCullRS;
	static ID3D11RasterizerState* CullClockwiseRS;
	static ID3D11RasterizerState* CullCounterClockwiseRS;
	static ID3D11RasterizerState* SolidFillRS;

	// Blend states
	static ID3D11BlendState* AlphaToCoverageBS;
	static ID3D11BlendState* TransparentBS;
	static ID3D11BlendState* NoRenderTargetWritesBS;
	static ID3D11BlendState* srsColor;


	// Depth/stencil states
	static ID3D11DepthStencilState* MarkMirrorDSS;
	static ID3D11DepthStencilState* DrawReflectionDSS;
	static ID3D11DepthStencilState* NoDoubleBlendDSS;
	static ID3D11DepthStencilState* DepthComplexityCountDSS;
	static ID3D11DepthStencilState* DepthComplexityReadDSS;




	void Bind(Graphics& gfx) noexcept override;
};

