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

	VS_ILCreateAndBind(pLightAndTextureVS, L"Shaders\\Vertex\\LightAndTextureVS.cso");
	
// 
// 	VertexShader* pVertexShader = new VertexShader(gfx, L"Shaders\\Vertex\\LightAndTextureVS.cso");
// 	pVABlob = pVertexShader->GetByteCode();
// 	gfx.pgfx_pDeviceContext->VSSetShader(pVertexShader, )

}

void Shaders::VS_ILCreateAndBind(ID3D11VertexShader* pVShader, const std::wstring& path)
{
#ifdef MY_DEBUG
	pSgfx->CheckFileExistence(pSgfx, path);
#endif // MY_DEBUG
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), &pVABlob));
	DX::ThrowIfFailed(GetDevice(*pSgfx)->CreateVertexShader(
		pVABlob->GetBufferPointer(),
		pVABlob->GetBufferSize(),
		nullptr,
		&pVShader));
#ifdef MY_DEBUG
	if (path != std::wstring())
	{
		pSgfx->SetDebugName(pVShader, path.c_str());
	}
#endif
	pSgfx->pgfx_pDeviceContext->VSSetShader(pVShader, nullptr, 0u);
	InputLayoutCreateAndBind(inputLightTexture, pVABlob, L"LightAndTextureVS_");


}

void Shaders::InputLayoutCreateAndBind(std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout, ID3DBlob* pBlob, const std::wstring& name)
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

	GetContext(*pSgfx)->IASetInputLayout(pLightAndTextureIL);
}

void Shaders::Bind(Graphics& gfx) noexcept
{

}
