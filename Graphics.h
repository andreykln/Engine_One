#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include "dxgidebug.h"
#include "directxmath.h"
#include <d3dcompiler.h>
#include "CustomException.h"


class Graphics
{
public:
	Graphics(HWND wnd);
	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;
	void TestDrawing()
	{
		struct Vertex
		{
			float x;
			float y;
		};
		const Vertex cubeCoord[]
		{
			{-1.0f, -1.0f},
			{ 1.0f, -1.0f},
			{-1.0f,  1.0f},
			{ 1.0f,  1.0f}
		};
		
		D3D11_INPUT_ELEMENT_DESC inputElemDesc[] =
		{
			"Position", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA, 0u
		};

		ID3D11Buffer* pVertexBuffer;
		UINT stride = sizeof(Vertex);

		pgfx_pDeviceContext->IASetVertexBuffers(0u, 1u, &pVertexBuffer, &stride, NULL);
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Device> pgfx_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pgfx_pDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pgfx_SwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pgfx_RenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pgfx_BackBuffer;

	D3D_FEATURE_LEVEL featureLevelIsSupported;
	UINT featureLevelNum = 7;
	D3D_FEATURE_LEVEL d3dFeatureLevels [7]
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};
};

