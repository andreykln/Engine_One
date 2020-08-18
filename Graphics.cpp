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
	//
	// RASTERIZER STATE TEST/////////////////////// This can work as a default state if needed
	////////
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pgfx_RasterState;
	D3D11_RASTERIZER_DESC rasterDesc;
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

	DX::ThrowIfFailed(pgfx_pDevice->CreateRasterizerState(&rasterDesc, pgfx_RasterState.ReleaseAndGetAddressOf()));
	pgfx_pDeviceContext->RSSetState(pgfx_RasterState.Get());
// 
// 
	//
	// RASTERIZER STATE END //////////////////////////
	//

	//DEPTH/STENCIL for mirror
	D3D11_DEPTH_STENCIL_DESC mirrorDepthDesc;
	mirrorDepthDesc.DepthEnable = TRUE;
	mirrorDepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	mirrorDepthDesc.DepthFunc = D3D11_COMPARISON_LESS;
	mirrorDepthDesc.StencilEnable = TRUE;
	mirrorDepthDesc.StencilReadMask = 0xff;
	mirrorDepthDesc.StencilWriteMask = 0xff;

	mirrorDepthDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDepthDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDepthDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	mirrorDepthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// We are not rendering back facing polygons, so these settings do not matter.
	mirrorDepthDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDepthDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDepthDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	mirrorDepthDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	pgfx_pDevice->CreateDepthStencilState(&mirrorDepthDesc, pMarkMirror.ReleaseAndGetAddressOf());
#ifdef MY_DEBUG
	SetDeviceDebugName(pMarkMirror.Get(), L"DepthStencilState.");
#endif
	//pgfx_pDeviceContext->OMSetDepthStencilState(pMarkMirror.Get(), 1u);

	// DEPTH/STENCIL BUFFER
	D3D11_DEPTH_STENCILOP_DESC stencilOperator;
	stencilOperator.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	stencilOperator.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	stencilOperator.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	stencilOperator.StencilFunc = D3D11_COMPARISON_ALWAYS;


	D3D11_DEPTH_STENCIL_DESC depth_description = {};
	depth_description.StencilEnable = TRUE;
	depth_description.DepthEnable = TRUE;
	depth_description.DepthFunc = D3D11_COMPARISON_LESS;
	depth_description.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_description.StencilReadMask = 0xff;
	depth_description.StencilWriteMask = 0xff;

	depth_description.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_description.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depth_description.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_description.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	// We are not rendering back facing polygons, so these settings do not matter. But it won't work without them
	depth_description.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_description.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depth_description.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_description.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	pgfx_pDevice->CreateDepthStencilState(&depth_description, pDrawReflectionState.ReleaseAndGetAddressOf());
#ifdef MY_DEBUG
	SetDeviceDebugName(pDrawReflectionState.Get(), L"DepthStencilState.");
#endif
	pgfx_pDeviceContext->OMSetDepthStencilState(pDrawReflectionState.Get(), 1u);

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
#ifdef MY_DEBUG
	SetDeviceDebugName(pgfx_TextureDepthStencil.Get(), L"Text2DDepthStentcil.");
#endif

	//create view of depth stencil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;

	DX::ThrowIfFailed(pgfx_pDevice->CreateDepthStencilView(
															pgfx_TextureDepthStencil.Get(),
															&descDSV,
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

void Graphics::SetProjection(DirectX::XMMATRIX in_projection) noexcept
{
	projection = in_projection;
}

void Graphics::DrawIndexed(UINT count) const noexcept
{
	pgfx_pDeviceContext->DrawIndexed(count, 0u, 0u);
}

void Graphics::Draw(UINT VertexCount, UINT StartVertexLocation) const noexcept
{
	pgfx_pDeviceContext->Draw(VertexCount, StartVertexLocation);
}

void Graphics::DrawIndexedTwo(UINT count, UINT StartIndexLocation, INT BaseVertexLocation) const noexcept
{
	pgfx_pDeviceContext->DrawIndexed(count, StartIndexLocation, BaseVertexLocation);
// 	pgfx_pDeviceContext->DrawIndexed(36u, 0u,0u);

}


HWND Graphics::GetWindowHandle() const noexcept
{
	return windowHandle;
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

#endif

DirectX::XMMATRIX Graphics::GetProjection() const noexcept
{
	return projection;
}

