#include "Graphics.h"
// #include "D3Dcommon.h"
Graphics::Graphics(HWND wnd)
{
	windowHandle = wnd;
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
#ifdef MY_DEBUG
		D3D11_CREATE_DEVICE_DEBUG,
#endif
#ifndef MY_DEBUG
		0u,
#endif // !MY_DEBUG
		d3dFeatureLevels, 
		featureLevelNum, 
		D3D11_SDK_VERSION,
		&swapChainDesc,
		pgfx_SwapChain.ReleaseAndGetAddressOf(),
		pgfx_pDevice.ReleaseAndGetAddressOf(),
		&featureLevelIsSupported, 
		pgfx_pDeviceContext.ReleaseAndGetAddressOf()));
#ifdef MY_DEBUG
	pgfx_pDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugDevice));
	SetDeviceDebugName(pgfx_pDeviceContext.Get(), L"DeviceContextCreation.");
#endif


	D3D11_TEXTURE2D_DESC descDepthTexture;
	descDepthTexture.Width = resolution_width;
	descDepthTexture.Height = resolution_height;
	descDepthTexture.MipLevels = 1u;
	descDepthTexture.ArraySize = 1u;
	descDepthTexture.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //	DXGI_FORMAT_D32_FLOAT DXGI_FORMAT_D24_UNORM_S8_UINT
	descDepthTexture.SampleDesc.Count = 1u;
	descDepthTexture.SampleDesc.Quality = 0u;
	descDepthTexture.Usage = D3D11_USAGE_DEFAULT;
	descDepthTexture.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepthTexture.CPUAccessFlags = 0;
	descDepthTexture.MiscFlags = 0;
	DX::ThrowIfFailed(pgfx_pDevice->CreateTexture2D(&descDepthTexture, 0u, pgfx_TextureDepthStencil.ReleaseAndGetAddressOf()));
#ifdef MY_DEBUG
	SetDeviceDebugName(pgfx_TextureDepthStencil.Get(), L"Text2DDepthStentcil.");
#endif

	DX::ThrowIfFailed(pgfx_pDevice->CreateDepthStencilView(
															pgfx_TextureDepthStencil.Get(),
															0u,
															pgfx_DepthStencilView.ReleaseAndGetAddressOf()));
#ifdef MY_DEBUG
	SetDeviceDebugName(pgfx_DepthStencilView.Get(), L"DepthStencilView.");
#endif

	//viewport
	vp.Width = resolution_width;
	vp.Height = resolution_height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

#ifdef MY_DEBUG
	ID3D11InfoQueue* infoQueue = nullptr;
	pgfx_pDevice->QueryInterface(__uuidof(ID3D11InfoQueue), reinterpret_cast<void**>(&infoQueue));
	if (infoQueue)
	{
		infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_WARNING, FALSE);
		infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, TRUE);
		infoQueue->Release();
		infoQueue = nullptr;
	}

#endif
	DX::ThrowIfFailed(pgfx_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pgfx_BackBuffer));
	DX::ThrowIfFailed(pgfx_pDevice->CreateRenderTargetView(pgfx_BackBuffer.Get(), nullptr, pgfx_RenderTargetView.ReleaseAndGetAddressOf()));
#ifdef MY_DEBUG
	SetDeviceDebugName(pgfx_RenderTargetView.Get(), L"CreateRenderTargetView.");
#endif
}

Graphics::~Graphics()
{
#ifdef MY_DEBUG
	if (debugDevice != nullptr)
	{
		debugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}
	debugDevice->Release();
	debugDevice = nullptr;
#endif
}

void Graphics::EndFrame()
{
	DX::ThrowIfFailed(pgfx_SwapChain->Present(0u, 0u));
	pgfx_pDeviceContext->OMSetRenderTargets(1u, pgfx_RenderTargetView.GetAddressOf(), pgfx_DepthStencilView.Get());
	pgfx_pDeviceContext->RSSetViewports(1u, &vp);
}


void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] = { red, green, blue, 1.0f };
	pgfx_pDeviceContext->ClearRenderTargetView(pgfx_RenderTargetView.Get(), color);
	pgfx_pDeviceContext->ClearDepthStencilView(pgfx_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}


void Graphics::DrawIndexed(UINT count) const noexcept
{
	pgfx_pDeviceContext->DrawIndexed(count, 0u, 0u);
}

void Graphics::DrawIndexed(UINT count, UINT startIndexLocation, UINT startVertexLocation)
{
	pgfx_pDeviceContext->DrawIndexed(count, startIndexLocation, startVertexLocation);
}

void Graphics::Draw(UINT VertexCount, UINT StartVertexLocation) const noexcept
{
	pgfx_pDeviceContext->Draw(VertexCount, StartVertexLocation);
}

void Graphics::DrawIndexedTwo(UINT count, UINT StartIndexLocation, INT BaseVertexLocation) const noexcept
{
	pgfx_pDeviceContext->DrawIndexed(count, StartIndexLocation, BaseVertexLocation);
}


void Graphics::DrawInstancedIndexed(UINT count, UINT instanceCount, UINT startIndexLocation, int baseVertexLocation, UINT startInstanceLocation)
{
	pgfx_pDeviceContext->DrawIndexedInstanced(count, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
}

HWND Graphics::GetWindowHandle() const noexcept
{
	return windowHandle;
}

void Graphics::SetViewport()
{
	pgfx_pDeviceContext->RSSetViewports(1u, &vp);
}


void Graphics::SetMatrices(const DirectX::XMMATRIX& ViewProjection, const DirectX::XMMATRIX& View)
{
	mView = View;
	mViewProjection = ViewProjection;
}

void Graphics::NormalMap(const DirectX::XMMATRIX world)
{

}

#ifdef MY_DEBUG
void Graphics::SetDebugName(ID3D11DeviceChild* child, const std::wstring& name)
{
	if (child != nullptr)
	{
		//convert wstring to const char*
		char* c_sName = new char[name.length()+2];
		c_sName[name.size()] = '\0';
		WideCharToMultiByte(CP_ACP, 0u, name.c_str(), -1, c_sName, (UINT)name.length(), NULL, NULL);

		child->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(name.size()) - 1, c_sName);
		delete[] c_sName;
	}
}

std::string Graphics::wstrTostr(const std::wstring& wstr)
{
	int size_needed = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.length() + 1), 0, 0, 0, 0);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.length() + 1), &strTo[0], size_needed, 0, 0);
	return strTo;
}

void Graphics::SetDeviceDebugName(ID3D11DeviceChild* child, const std::wstring& name)
{
	SetDebugName(child, name);
}

void Graphics::CheckFileExistence(Graphics& gfx, const std::wstring& path)
{
	if (!std::filesystem::exists(path.c_str()))
	{
		//can't append string literal message to this string for some reason. 
		//I'll leave it like that, it's better than nothing
		MessageBox(gfx.GetWindowHandle(), gfx.wstrTostr(path).c_str(), NULL, MB_OK);
	}
}

void Graphics::CheckFileExistence(Graphics* gfx, const std::wstring& path)
{
	if (!std::filesystem::exists(path.c_str()))
	{
		//can't append string literal message to this string for some reason. 
		//I'll leave it like that, it's better than nothing
		MessageBox(gfx->GetWindowHandle(), gfx->wstrTostr(path).c_str(), NULL, MB_OK);
	}
}

#endif



