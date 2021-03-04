#pragma once
#include "Bindable.h"
#include "UtilityStructures.h"
#include "Shape.h"
#include "BindableBase.h"


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
	void VS_IL_Init(ID3D11VertexShader** pVShader, const std::wstring& path);
	void PS_Init(ID3D11PixelShader** pPSShader, const std::wstring& path);
	void InitializeInputLayout(std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout, ID3DBlob* pBlob, const std::wstring& name);
	Graphics* pSgfx = nullptr;

	ID3D11VertexShader* pBillboardVS = nullptr;
	ID3D11VertexShader* pLightAndTextureVS = nullptr;

	ID3D11PixelShader* pLightAndTextuerPS = nullptr;

	ID3DBlob* pBlob = nullptr;
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLightTexture;
	ID3D11InputLayout* pLightAndTextureIL = nullptr;

};

