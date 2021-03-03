#include "Shaders.h"

Shaders::Shaders(Graphics& in_gfx)
	: pSgfx(&in_gfx)
{
	inputLightTexture =
	{
		{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"Normal", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, sizeof(DirectX::XMFLOAT3),
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"TexCoordinate", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, sizeof(DirectX::XMFLOAT3) * 2,
		D3D11_INPUT_PER_VERTEX_DATA, 0u}
	};

	VS_IL_Init(&pLightAndTextureVS, L"Shaders\\Vertex\\LightAndTextureVS.cso");

}

void Shaders::BindVSandIA(ShaderPicker shader)
{
	switch (shader)
	{
	case ShaderPicker::LightAndTexture_VS_PS:
		{
			pSgfx->pgfx_pDeviceContext->VSSetShader(pLightAndTextureVS, nullptr, 0u);
			GetContext(*pSgfx)->IASetInputLayout(pLightAndTextureIL);
		}
	}
}

void Shaders::VS_IL_Init(ID3D11VertexShader** pVShader, const std::wstring& path)
{
#ifdef MY_DEBUG
	pSgfx->CheckFileExistence(pSgfx, path);
#endif // MY_DEBUG
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), &pVABlob));
	DX::ThrowIfFailed(GetDevice(*pSgfx)->CreateVertexShader(
		pVABlob->GetBufferPointer(),
		pVABlob->GetBufferSize(),
		nullptr,
		pVShader));
#ifdef MY_DEBUG
	if (path != std::wstring())
	{
		pSgfx->SetDebugName(*pVShader, path.c_str());
	}
#endif
	InitializeInputLayout(inputLightTexture, pVABlob, L"LightAndTextureVS_");


}

void Shaders::InitializeInputLayout(std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout, ID3DBlob* pBlob, const std::wstring& name)
{
	DX::ThrowIfFailed(GetDevice(*pSgfx)->CreateInputLayout(
		inputLayout.data(),
		static_cast<UINT>(inputLayout.size()),
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
