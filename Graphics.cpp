#include "Graphics.h"

Graphics::Graphics(HWND wnd)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc{ 0 };
	swapChainDesc.BufferDesc.Width = 0;
	swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1u;
	swapChainDesc.SampleDesc.Quality = 0u;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2u; 
	swapChainDesc.OutputWindow = wnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapChainDesc.Flags = 0;
	
	DX::ThrowIfFailed(D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		d3dFeatureLevels, 
		featureLevelNum, 
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&pgfx_SwapChain,
		&pgfx_pDevice,
		&featureLevelIsSupported, 
		&pgfx_pDeviceContext));

	// DEPTH BUFFER
	D3D11_DEPTH_STENCIL_DESC depth_description = {};
	depth_description.StencilEnable = FALSE;
	depth_description.DepthEnable = TRUE;
	depth_description.DepthFunc = D3D11_COMPARISON_LESS;
	depth_description.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;
	pgfx_pDevice->CreateDepthStencilState(&depth_description, pDSState.ReleaseAndGetAddressOf());
	pgfx_pDeviceContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	D3D11_TEXTURE2D_DESC descDepthTexture;
	descDepthTexture.Width = resolution_width;
	descDepthTexture.Height = resolution_height;
	descDepthTexture.MipLevels = 1u;
	descDepthTexture.ArraySize = 1u;
	descDepthTexture.Format = DXGI_FORMAT_D32_FLOAT;
	descDepthTexture.SampleDesc.Count = 1u;
	descDepthTexture.SampleDesc.Quality = 0u;
	descDepthTexture.Usage = D3D11_USAGE_DEFAULT;
	descDepthTexture.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepthTexture.CPUAccessFlags = 0;
	descDepthTexture.MiscFlags = 0;
	DX::ThrowIfFailed(pgfx_pDevice->CreateTexture2D(&descDepthTexture, 0u, pgfx_TextureDepthStencil.ReleaseAndGetAddressOf()));

	//create view of depth stencil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;

	DX::ThrowIfFailed(pgfx_pDevice->CreateDepthStencilView(
								pgfx_TextureDepthStencil.Get(),
								&descDSV,
								pgfx_DepthStencilView.ReleaseAndGetAddressOf()));

	vp.Width = resolution_width;
	vp.Height = resolution_height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	DX::ThrowIfFailed(pgfx_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pgfx_BackBuffer));
	DX::ThrowIfFailed(pgfx_pDevice->CreateRenderTargetView(pgfx_BackBuffer.Get(), nullptr, &pgfx_RenderTargetView));
}

void Graphics::EndFrame()
{
	DX::ThrowIfFailed(pgfx_SwapChain->Present(1u, 0u));
	pgfx_pDeviceContext->OMSetRenderTargets(1u, pgfx_RenderTargetView.GetAddressOf(),pgfx_DepthStencilView.Get());
	pgfx_pDeviceContext->RSSetViewports(1u, &vp);
}


void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] = { red, green, blue, 1.0f };
	pgfx_pDeviceContext->ClearRenderTargetView(pgfx_RenderTargetView.Get(), color);
	pgfx_pDeviceContext->ClearDepthStencilView(pgfx_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Graphics::SetProjection(DirectX::XMMATRIX in_projection) noexcept
{
	projection = in_projection;
}

void Graphics::DrawIndexed(UINT count) const noexcept
{
	pgfx_pDeviceContext->DrawIndexed(count, 0u, 0u);
}

DirectX::XMMATRIX Graphics::GetProjection() const noexcept
{
	return projection;
}

