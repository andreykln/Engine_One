#pragma once
#define NO_STRICT
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include "dxgidebug.h"
#include "directxmath.h"
#include <d3dcompiler.h>
#include "CustomException.h"

class Window;

class Graphics
{
public:
	Microsoft::WRL::ComPtr<ID3D11Device> gfx_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> gfx_pDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> gfx_SwapChain;



	Microsoft::WRL::ComPtr<IDXGIDevice> idxgi_Device;
	Microsoft::WRL::ComPtr<IDXGIAdapter> idxgi_Adapter;
	Microsoft::WRL::ComPtr<IDXGIFactory> idxgi_Factory;


	HWND wnd;
	Graphics(HWND wnd);


private:
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

