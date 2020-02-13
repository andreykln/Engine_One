#include "Graphics.h"

Graphics::Graphics(HWND wnd)
{
	DXGI_MODE_DESC displayModeDesc;
	displayModeDesc.Height = 0u;
	displayModeDesc.Width = 0u;
	displayModeDesc.RefreshRate.Numerator = 60;
	displayModeDesc.RefreshRate.Denominator = 1;
	displayModeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	displayModeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	displayModeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc = displayModeDesc;
	swapChainDesc.SampleDesc.Count = 1u;
	swapChainDesc.SampleDesc.Quality = 1u;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = DXGI_SWAP_EFFECT_SEQUENTIAL;
	swapChainDesc.OutputWindow = wnd;
	swapChainDesc.Windowed = FALSE;

	IDXGIFactory::CreateSwapChain(gfx_pDevice, &swapChainDesc, &gfx_SwapChain);


	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		d3dFeatureLevels,
		7u,
		D3D11_SDK_VERSION,
		&swapChainDesc,


		)
}
