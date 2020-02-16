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
	Microsoft::WRL::ComPtr<ID3D11Device> pgfx_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pgfx_pDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pgfx_SwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pgfx_RenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pgfx_BackBuffer;






	Microsoft::WRL::ComPtr<IDXGIDevice> idxgi_Device;
	Microsoft::WRL::ComPtr<IDXGIAdapter> idxgi_Adapter;
	Microsoft::WRL::ComPtr<IDXGIFactory> idxgi_Factory;


	Graphics(HWND wnd);
public:
	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;

private:
	D3D_FEATURE_LEVEL featureLevel{};
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

