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

	VS_IL_Init(&pDepthComplexityVS, IL.positionColorIL, &pDepthCoplexityIL, IL.nPosition_Color, L"Shaders\\Vertex\\DepthCompVS.cso");
	PS_Init(&pDepthComplexityPS, L"Shaders\\Pixel\\DepthCompPS.cso");

	VS_IL_Init(&pTreeBillboardVS, IL.treeBillboardIL, &pTreeBillboardIL, IL.nTreeBillboardElements, L"Shaders\\Vertex\\TreeBillboardVS.cso");
	GS_Init(&pTreeBillboardGS, L"Shaders\\Geometry\\TreeBillboardGS.cso");
	PS_Init(&pTreeBillboardPS, L"Shaders\\Pixel\\TreeBillboardPS.cso");

	VS_IL_Init(&pCirletoCylinderVS, IL.positonIL, &pCirletoCylinderIL, IL.nPositon, L"Shaders\\Vertex\\Circle_CylinderVS.cso");
	GS_Init(&pCircletoCylinderGS, L"Shaders\\Geometry\\Circle_CylinderGS.cso");
	PS_Init(&pCirletoCylinderPS, L"Shaders\\Pixel\\Circle_CylinderPS.cso");

	PS_Init(&pMirrorSkullPS, L"Shaders\\Pixel\\SkullMirrorPS.cso");
	PS_Init(&pAllLightForHills, L"Shaders\\Pixel\\HillsAllLightPS.cso");

	CS_Init(&pHorizontalBlurCS, L"Shaders\\Compute\\HorizontalGaussianBlurCS.cso");
	CS_Init(&pVerticalBlurCS, L"Shaders\\Compute\\VerticalGaussianBlurCS.cso");
	PS_Init(&pBlurTexturePS, L"Shaders\\Pixel\\BlurTexturePS.cso");

	CS_Init(&pHorizontalBilateralBlur, L"Shaders\\Compute\\HorizontalBilateralBlur.cso");
	CS_Init(&pVerticalBilateralBlur, L"Shaders\\Compute\\VerticalBilateralBlur.cso");

	CS_Init(&pDisturbWaves, L"Shaders\\Compute\\DisturbWavesCS.cso");
	CS_Init(&pUpdateWaves, L"Shaders\\Compute\\UpdateWavesCS.cso");
	VS_IL_Init(&pGPUWavesVS, IL.lightTexture, &pLightAndTextureIL,
		IL.nLightTextureElements, L"Shaders\\Vertex\\GPUWavesVS.cso");

	VS_IL_Init(&pQuadTessellationVS, IL.positonIL, &pPositonIL, IL.nPositon, L"Shaders\\Vertex\\QuadTessellationPassVS.cso");
	PS_Init(&pQuadTessellationPS, L"Shaders\\Pixel\\QuadTessPS.cso");
	HS_Init(&pQuadTesselationHS, L"Shaders\\Hull\\QuadTessHS.cso");
	DS_Init(&pQuadTesselationDS, L"Shaders\\Domain\\QuadTessDS.cso");

	VS_IL_Init(&pInstancedSkullVS, IL.instancedSkull, &pInstancedSkullIL, IL.nInstancedSkull, L"Shaders\\Vertex\\InstancedSkullVS.cso");
	PS_Init(&pInstancedSkullPS, L"Shaders\\Pixel\\InstancedSkullPS.cso");

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
	case ShaderPicker::TreeBillboardVS_PS_GS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pTreeBillboardIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pTreeBillboardVS, nullptr, 0u);
		
		break;
	}
	case ShaderPicker::CircleToCylinderVS_GS_PS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pCirletoCylinderIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pCirletoCylinderVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::GPUWaves_VS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pLightAndTextureIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pGPUWavesVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::QuadTessellation_VS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pPositonIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pQuadTessellationVS, nullptr, 0u);
		break;
	}
	case ShaderPicker::InstancedSkull_VS:
	{
		GetContext(*pSgfx)->IASetInputLayout(pInstancedSkullIL);
		pSgfx->pgfx_pDeviceContext->VSSetShader(pInstancedSkullVS, nullptr, 0u);
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
	case ShaderPicker::TreeBillboardVS_PS_GS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pTreeBillboardPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::CircleToCylinderVS_GS_PS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pCirletoCylinderPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::MirrorSkull_PS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pMirrorSkullPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::HillsAllLight_PS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pAllLightForHills, nullptr, 0u);
		break;
	}
	case ShaderPicker::BlurTexture_PS :
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pBlurTexturePS, nullptr, 0u);
		break;
	}
	case ShaderPicker::QuadTessellation_PS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pQuadTessellationPS, nullptr, 0u);
		break;
	}
	case ShaderPicker::InstancedSkull_PS:
	{
		pSgfx->pgfx_pDeviceContext->PSSetShader(pInstancedSkullPS, nullptr, 0u);
		break;
	}
	default:
		break;
	}
}

void Shaders::BindGS(ShaderPicker shader)
{
	switch (shader)
	{
	case ShaderPicker::TreeBillboardVS_PS_GS:
	{
		pSgfx->pgfx_pDeviceContext->GSSetShader(pTreeBillboardGS, nullptr, 0u);
		break;
	}
	case ShaderPicker::CircleToCylinderVS_GS_PS:
	{
		pSgfx->pgfx_pDeviceContext->GSSetShader(pCircletoCylinderGS, nullptr, 0u);
		break;
	}
	default:
		break;
	}
}

void Shaders::BindCS(ShaderPicker shader)
{
	switch (shader)
	{
	case ShaderPicker::HorizontalBlur_CS:
	{
		pSgfx->pgfx_pDeviceContext->CSSetShader(pHorizontalBlurCS, nullptr, 0u);
		break;
	}
	case ShaderPicker::VerticalBlur_CS:
	{
		pSgfx->pgfx_pDeviceContext->CSSetShader(pVerticalBlurCS, nullptr, 0u);
		break;
	}
	case ShaderPicker::HorizontalBilateralBlur_CS:
	{
		pSgfx->pgfx_pDeviceContext->CSSetShader(pHorizontalBilateralBlur, nullptr, 0u);
		break;
	}
	case ShaderPicker::VerticalBilateralBlur_CS:
	{
		pSgfx->pgfx_pDeviceContext->CSSetShader(pVerticalBilateralBlur, nullptr, 0u);
		break;
	}
	case ShaderPicker::DisturbWaves_CS:
	{
		pSgfx->pgfx_pDeviceContext->CSSetShader(pDisturbWaves, nullptr, 0u);
		break;
	}
	case ShaderPicker::UpdateWaves_CS:
	{
		pSgfx->pgfx_pDeviceContext->CSSetShader(pUpdateWaves, nullptr, 0u);
		break;
	}
	}
}

void Shaders::BindHS(ShaderPicker shader)
{
	switch (shader)
	{
	case ShaderPicker::QuadTessellation_HS:
	{
		pSgfx->pgfx_pDeviceContext->HSSetShader(pQuadTesselationHS, 0u, 0u);
		break;
	}
	}
}

void Shaders::BindDS(ShaderPicker shader)
{
	switch (shader)
	{
	case ShaderPicker::QuadTessellation_DS:
	{
		pSgfx->pgfx_pDeviceContext->DSSetShader(pQuadTesselationDS, 0u, 0u);
		break;
	}
	}
}

void Shaders::UnbindCS()
{
	pSgfx->pgfx_pDeviceContext->CSSetShader(0u, nullptr, 0u);
}

void Shaders::UnbindGS()
{
	pSgfx->pgfx_pDeviceContext->GSSetShader(0u, nullptr, 0u);
}

void Shaders::UnbindPS()
{
	pSgfx->pgfx_pDeviceContext->CSSetShader(0u, nullptr, 0u);
}

void Shaders::UnbindVS()
{
	pSgfx->pgfx_pDeviceContext->VSSetShader(0u, nullptr, 0u);
}

void Shaders::UnbindHS()
{
	pSgfx->pgfx_pDeviceContext->HSSetShader(0u, nullptr, 0u);
}

void Shaders::UnbindDS()
{
	pSgfx->pgfx_pDeviceContext->DSSetShader(0u, nullptr, 0u);
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
	//for usage in other shaders;
	pBlob->Release();

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

void Shaders::GS_Init(ID3D11GeometryShader** pGSShader, const std::wstring& path)
{
#ifdef MY_DEBUG
	pSgfx->CheckFileExistence(pSgfx, path);
#endif // MY_DEBUG
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), &pBlob));
	DX::ThrowIfFailed(GetDevice(*pSgfx)->CreateGeometryShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		pGSShader));
#ifdef MY_DEBUG
	if (GetDebug(*pSgfx) != nullptr)
	{
		pSgfx->SetDebugName(*pGSShader, path.c_str());
	}
#endif
	//for usage in other shaders;
	pBlob->Release();
}

void Shaders::CS_Init(ID3D11ComputeShader** pCShader, const std::wstring& path)
{
#ifdef MY_DEBUG
	pSgfx->CheckFileExistence(pSgfx, path);
#endif // MY_DEBUG
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), &pBlob));
	DX::ThrowIfFailed(GetDevice(*pSgfx)->CreateComputeShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		pCShader));
#ifdef MY_DEBUG
	if (GetDebug(*pSgfx) != nullptr)
	{
		pSgfx->SetDebugName(*pCShader, path.c_str());
	}
#endif
	//for usage in other shaders;
	pBlob->Release();
}

void Shaders::HS_Init(ID3D11HullShader** pHShader, const std::wstring& path)
{
#ifdef MY_DEBUG
	pSgfx->CheckFileExistence(pSgfx, path);
#endif // MY_DEBUG
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), &pBlob));
	DX::ThrowIfFailed(GetDevice(*pSgfx)->CreateHullShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		pHShader));
#ifdef MY_DEBUG
	if (GetDebug(*pSgfx) != nullptr)
	{
		pSgfx->SetDebugName(*pHShader, path.c_str());
	}
#endif
	//for usage in other shaders;
	pBlob->Release();

}

void Shaders::DS_Init(ID3D11DomainShader** pDshader, const std::wstring& path)
{
#ifdef MY_DEBUG
	pSgfx->CheckFileExistence(pSgfx, path);
#endif // MY_DEBUG
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), &pBlob));
	DX::ThrowIfFailed(GetDevice(*pSgfx)->CreateDomainShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		pDshader));
#ifdef MY_DEBUG
	if (GetDebug(*pSgfx) != nullptr)
	{
		pSgfx->SetDebugName(*pDshader, path.c_str());
	}
#endif
	//for usage in other shaders;
	pBlob->Release();

}

void Shaders::Bind(Graphics& gfx) noexcept
{

}
