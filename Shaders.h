#pragma once
#include "Bindable.h"
#include "UtilityStructures.h"
#include "Shape.h"
#include "BindableBase.h"
struct InputLayouts
{
	static const UINT nLightTextureElements = 3;
	const D3D11_INPUT_ELEMENT_DESC lightTexture[nLightTextureElements] =
	{
		{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"Normal", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, sizeof(DirectX::XMFLOAT3),
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"TexCoordinate", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, sizeof(DirectX::XMFLOAT3) * 2,
		D3D11_INPUT_PER_VERTEX_DATA, 0u}
	};

};
class Shaders : public Bindable
{
public:
	Shaders(Graphics& in_gfx);
	Shaders(const Shaders&) = delete;
	Shaders& operator= (const Shaders&) = delete;


	void BindVSandIA(ShaderPicker shader);
	void BindPS(ShaderPicker shader);

	//most convenient to have this class to be a child of Bindable
	//but this function is empty
	void Bind(Graphics& gfx) noexcept override; 
private:
	void VS_IL_Init(ID3D11VertexShader** pVShader, const D3D11_INPUT_ELEMENT_DESC* inputLayout, UINT nElements, const std::wstring& path);
	void PS_Init(ID3D11PixelShader** pPSShader, const std::wstring& path);
	void InitializeInputLayout(const D3D11_INPUT_ELEMENT_DESC* inputLayout, UINT nElements,ID3DBlob* pBlob, const std::wstring& name);
	Graphics* pSgfx = nullptr;
	InputLayouts IL;
	ID3D11VertexShader* pBillboardVS = nullptr;
	ID3D11VertexShader* pLightAndTextureVS = nullptr;

	ID3D11PixelShader* pLightAndTextuerPS = nullptr;

	ID3DBlob* pBlob = nullptr;
	ID3D11InputLayout* pLightAndTextureIL = nullptr;

};

