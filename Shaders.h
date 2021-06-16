#pragma once
#include "Bindable.h"
#include "UtilityStructures.h"
#include "Shape.h"
#include "BindableBase.h"
#include "InputLayout.h"

class Shaders : public Bindable
{
public:
	Shaders(Graphics& in_gfx);
	Shaders(const Shaders&) = delete;
	Shaders& operator= (const Shaders&) = delete;

	void BindVSandIA(ShaderPicker shader);
	void BindPS(ShaderPicker shader);
	void BindGS(ShaderPicker shader);
	void BindCS(ShaderPicker shader);
	void BindHS(ShaderPicker shader);
	void BindDS(ShaderPicker shader);

	void UnbindCS();
	void UnbindGS();
	void UnbindPS();
	void UnbindVS();
	void UnbindHS();
	void UnbindDS();
private:
	//most convenient to have this class to be a child of Bindable
//but this function isn't needed
	void Bind(Graphics& gfx) noexcept override; 
	void VS_IL_Init(ID3D11VertexShader** pVShader, const D3D11_INPUT_ELEMENT_DESC* inputLayout,
		ID3D11InputLayout** pIL, UINT nElements, const std::wstring& path);
	void PS_Init(ID3D11PixelShader** pPSShader, const std::wstring& path);
	void InitializeInputLayout(const D3D11_INPUT_ELEMENT_DESC* inputLayout, ID3D11InputLayout** pIL,
		UINT nElements,ID3DBlob* pBlob, const std::wstring& name);
	void GS_Init(ID3D11GeometryShader** pGSShader, const std::wstring& path);
	void CS_Init(ID3D11ComputeShader** pCShader, const std::wstring& path);
	void HS_Init(ID3D11HullShader** pHShader, const std::wstring& path);
	void DS_Init(ID3D11DomainShader** pDshader, const std::wstring& path);

	Graphics* pSgfx = nullptr;
	InputLayout IL;

	ID3D11VertexShader* pLightAndTextureVS = nullptr;
	ID3D11InputLayout* pLightAndTextureIL = nullptr;
	ID3D11PixelShader* pLightAndTexturePS = nullptr;
	ID3D11PixelShader* pLightAndTextureArrayPS = nullptr;
	ID3D11PixelShader* pAllLightForHills = nullptr;

	ID3D11VertexShader* pDepthComplexityVS = nullptr;
	ID3D11InputLayout* pDepthCoplexityIL = nullptr;
	ID3D11PixelShader*  pDepthComplexityPS = nullptr;

	ID3D11VertexShader* pLightVS = nullptr;
	ID3D11InputLayout* pLightIL = nullptr;
	ID3D11PixelShader* pLightPS = nullptr;

	ID3D11VertexShader* pTreeBillboardVS = nullptr;
	ID3D11InputLayout* pTreeBillboardIL = nullptr;
	ID3D11PixelShader* pTreeBillboardPS = nullptr;
	ID3D11GeometryShader* pTreeBillboardGS = nullptr;

	ID3D11PixelShader* pMirrorRoomPS = nullptr;

	ID3D11VertexShader* pCirletoCylinderVS = nullptr;
	ID3D11InputLayout* pCirletoCylinderIL = nullptr;
	ID3D11GeometryShader* pCircletoCylinderGS = nullptr;
	ID3D11PixelShader* pCirletoCylinderPS = nullptr;

	ID3D11PixelShader* pMirrorSkullPS = nullptr;

	ID3DBlob* pBlob = nullptr;

	ID3D11ComputeShader* pHorizontalBlurCS = nullptr;
	ID3D11ComputeShader* pVerticalBlurCS = nullptr;
	ID3D11PixelShader* pBlurTexturePS = nullptr;

	ID3D11ComputeShader* pHorizontalBilateralBlur = nullptr;
	ID3D11ComputeShader* pVerticalBilateralBlur = nullptr;

	ID3D11ComputeShader* pDisturbWaves = nullptr;
	ID3D11ComputeShader* pUpdateWaves = nullptr;
	ID3D11VertexShader* pGPUWavesVS = nullptr;

	ID3D11VertexShader* pQuadTessellationVS = nullptr;
	ID3D11PixelShader* pQuadTessellationPS = nullptr;
	ID3D11HullShader* pQuadTesselationHS = nullptr;
	ID3D11DomainShader* pQuadTesselationDS = nullptr;

	//instanced skull
	ID3D11VertexShader* pInstancedSkullVS = nullptr;
	ID3D11PixelShader* pInstancedSkullPS = nullptr;
	ID3D11InputLayout* pInstancedSkullIL = nullptr;

	ID3D11InputLayout* pPositonIL = nullptr;

	//sky
	ID3D11VertexShader* pSkyVS = nullptr;
	ID3D11InputLayout* pSkyIL = nullptr;
	ID3D11PixelShader* pSkyPS = nullptr;
};

