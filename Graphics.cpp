#include "Graphics.h"

Graphics::Graphics(HWND wnd)
{
	DXGI_MODE_DESC displayModeDesc;
	displayModeDesc.Height = 0;
	displayModeDesc.Width = 0;
	displayModeDesc.RefreshRate.Numerator = 0;
	displayModeDesc.RefreshRate.Denominator = 0;
	displayModeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	displayModeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	displayModeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc = displayModeDesc;
	swapChainDesc.SampleDesc.Count = 1u;
	swapChainDesc.SampleDesc.Quality = 0u;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2u; //DXGI_SWAP_EFFECT_SEQUENTIAL try to see if this throws error
	swapChainDesc.OutputWindow = wnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapChainDesc.Flags = 0;
	
	D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr, //d3dFeatureLevels
		0u, //related to d3dFeatureLevels
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&pgfx_SwapChain,
		&pgfx_pDevice,
		&featureLevel, //TODO make sure that DX 11.1 feature level is being enabled.
		&pgfx_pDeviceContext);

	pgfx_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pgfx_BackBuffer);

	pgfx_pDevice->CreateRenderTargetView(pgfx_BackBuffer.Get(), nullptr, &pgfx_RenderTargetView);
	//const float colors[4]{ 0.5f, 0.5f, 0.2f };
	//pgfx_pDeviceContext->ClearRenderTargetView(pgfx_RenderTargetView.Get(), colors);


	
		
}

void Graphics::EndFrame()
{
	pgfx_SwapChain->Present(1u, 0u);
	pgfx_pDeviceContext->OMSetRenderTargets(1u, pgfx_RenderTargetView.GetAddressOf(),NULL);

}
void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] = { red, green, blue, 1.0f };
	pgfx_pDeviceContext->ClearRenderTargetView(pgfx_RenderTargetView.Get(), color);
}