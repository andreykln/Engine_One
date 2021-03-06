#include "Shaders.h"

Shaders::Shaders(Graphics& in_gfx)
	: pSgfx(&in_gfx)
{
	VS_IL_Init(&pLightAndTextureVS, IL.lightTexture, &pLightAndTextureIL,
				IL.nLightTextureElements, L"Shaders\\Vertex\\LightAndTextureVS.cso");
	PS_Init(&pLightAndTexturePS, L"Shaders\\Pixel\\LightAndTexturePS.cso");

	VS_IL_Init(&pLightVS, IL.light, &pLightIL, IL.nLightElements, L"Shaders\\Vertex\\LightVS.cso");
	PS_Init(&pLightPS, L"Shaders\\Pixel\\LightPS.cso");

	PS_Init(&pMirrorRoomPS, L"Shaders\\Pixel\\MirrorRoomPS.cso");

	PS_Init(&pLightAndTextureArrayPS, L"Shaders\\Pixel\\LightAndTextureArrayPS.cso");

	VS_IL_Init(&pDepthComplexityVS, IL.depthComplexityIL, &pDepthCoplexityIL, IL.nDepthComplElements, L"Shaders\\Vertex\\DepthCompVS.cso");
	PS_Init(&pDepthComplexityPS, L"Shaders\\Pixel\\DepthCompPS.cso");
}

void Shaders::BindVSandIA(DemoSwitch demo)
{
	switch (demo)
	{
	case DemoSwitch::DefaultBox:
		{
			pSgfx->pgfx_pDeviceContext->VSSetShader(pLightAndTextureVS, nullptr, 0u);
			GetContext(*pSgfx)->IASetInputLayout(pLightAndTextureIL);
			break;
		}
	}
}

void Shaders::BindVSandIA(ShaderPicker shader)
{
	switch (shader)
	{
	case ShaderPicker::LightAndTexture_VS_PS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pLightAndTextureIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pLightAndTextureVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::Light_VS_PS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pLightIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pLightVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::DepthComplexityVS_PS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pDepthCoplexityIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pDepthComplexityVS, nullptr, 0u);
		break;
	}
	default:
	break;
	}
}

void Shaders::BindPS(DemoSwitch demo)
{
	switch (demo)
	{
	case DemoSwitch::DefaultBox:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pLightAndTexturePS, nullptr, 0u);
		break;
	}
	default:
		break;
	}
}

void Shaders::BindPS(ShaderPicker shader)
{
	switch (shader)
	{
	case ShaderPicker::LightAndTexture_VS_PS:
		{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pLightAndTexturePS, nullptr, 0u);
		break;
		}
	case ShaderPicker::Light_VS_PS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pLightPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::MirrorRoomPS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pMirrorRoomPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::LightAndTextureArrayPS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pLightAndTextureArrayPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::DepthComplexityVS_PS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pDepthComplexityPS, nullptr, 0u);
		break;
	}
	default:
		break;
	}
}

void Shaders::VS_IL_Init(ID3D11VertexShader** pVShader,
						const D3D11_INPUT_ELEMENT_DESC* inputLayout,
						ID3D11InputLayout** pIL,
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
	InitializeInputLayout(inputLayout,pIL ,nElements, pBlob, L"VertexShader_");

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

void Shaders::InitializeInputLayout(const D3D11_INPUT_ELEMENT_DESC* inputLayout,
									ID3D11InputLayout** pIL, 
									UINT nElements,
									ID3DBlob* pBlob, const std::wstring& name)
{
	DX::ThrowIfFailed(GetDevice(*pSgfx)->CreateInputLayout(
		inputLayout,
		nElements,
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		pIL));
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
