#include "Shaders.h"

Shaders::Shaders(Graphics& in_gfx)
	: pSgfx(&in_gfx)
{
	VS_IL_Init(&pLightAndTextureVS, IL.lightTexture, IL.nLightTextureElements, L"Shaders\\Vertex\\LightAndTextureVS.cso");
	PS_Init(&pLightAndTextuerPS, L"Shaders\\Pixel\\LightAndTexturePS.cso");
}

void Shaders::BindVSandIA(ShaderPicker shader)
{
	switch (shader)
	{
	case ShaderPicker::LightAndTexture_VS_PS:
		{
			pSgfx->pgfx_pDeviceContext->VSSetShader(pLightAndTextureVS, nullptr, 0u);
			GetContext(*pSgfx)->IASetInputLayout(pLightAndTextureIL);
			break;
		}
	}
}

void Shaders::BindPS(ShaderPicker shader)
{
	switch (shader)
	{
	case ShaderPicker::LightAndTexture_VS_PS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pLightAndTextuerPS, nullptr, 0u);
	}
		break;
	default:
		break;
	}
}

void Shaders::VS_IL_Init(ID3D11VertexShader** pVShader, const D3D11_INPUT_ELEMENT_DESC* inputLayout,
						UINT nElements, const std::wstring& path)
{
#ifdef MY_DEBUG
	pSgfx->CheckFileExistence(pSgfx, path);
#endif // MY_DEBUG
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), &pBlob));
	DX::ThrowIfFailed(GetDevice(*pSgfx)->CreateVertexShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		pVShader));
#ifdef MY_DEBUG
	if (path != std::wstring())
	{
		pSgfx->SetDebugName(*pVShader, path.c_str());
	}
#endif
	auto name = path;
	InitializeInputLayout(inputLayout, nElements, pBlob, L"LightAndTextureVS_");

	//for usage in other shaders;
	pBlob->Release();

}

void Shaders::PS_Init(ID3D11PixelShader** pPSShader, const std::wstring& path)
{
#ifdef MY_DEBUG
	pSgfx->CheckFileExistence(pSgfx, path);
#endif // MY_DEBUG
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), &pBlob));
	DX::ThrowIfFailed(GetDevice(*pSgfx)->CreatePixelShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		pPSShader));
#ifdef MY_DEBUG
	if (GetDebug(*pSgfx) != nullptr)
	{
		pSgfx->SetDebugName(*pPSShader, path.c_str());
	}
#endif
}

void Shaders::InitializeInputLayout(const D3D11_INPUT_ELEMENT_DESC* inputLayout, UINT nElements, ID3DBlob* pBlob, const std::wstring& name)
{
	DX::ThrowIfFailed(GetDevice(*pSgfx)->CreateInputLayout(
		inputLayout,
		nElements,
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pLightAndTextureIL));
#ifdef MY_DEBUG
	if (name != std::wstring())
	{
		pSgfx->SetDebugName(pLightAndTextureIL, name.c_str());
	}
#endif
}

void Shaders::Bind(Graphics& gfx) noexcept
{

}
