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
	void UnbindAll();
private:
	//most convenient to have this class to be a child of Bindable
	//but this function isn't needed
	void Bind(Graphics& gfx) noexcept override; 

	void VS_IL_Init(ID3D11VertexShader** pVShader, const D3D11_INPUT_ELEMENT_DESC* inputLayout,
		ID3D11InputLayout** pIL, UINT nElements, const std::wstring& path);
	void VS_Init(ID3D11VertexShader** pVShader, const std::wstring& path);
	void PS_Init(ID3D11PixelShader** pPSShader, const std::wstring& path);
	void InitializeInputLayout(const D3D11_INPUT_ELEMENT_DESC* inputLayout, ID3D11InputLayout** pIL,
		UINT nElements,ID3DBlob* pBlob, const std::wstring& name);
	void GS_Init(ID3D11GeometryShader** pGSShader, const std::wstring& path);
	void GS_SO_Init(ID3D11GeometryShader** pGSShader, const std::wstring& path);
	void CS_Init(ID3D11ComputeShader** pCShader, const std::wstring& path);
	void HS_Init(ID3D11HullShader** pHShader, const std::wstring& path);
	void DS_Init(ID3D11DomainShader** pDshader, const std::wstring& path);

	Graphics* pSgfx = nullptr;
	InputLayout IL;
	ID3DBlob* pBlob = nullptr;
	Stream_Out SO;

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

	ID3D11PixelShader* pCubeMapsPS = nullptr;

	//normalMapping
	ID3D11VertexShader* pNormalMappingVS = nullptr;
	ID3D11PixelShader* pNormalMappingPS = nullptr;
	ID3D11InputLayout* pNormalMappingIL = nullptr;

	//displacement mapping. 
	ID3D11VertexShader* pDisplacementMappingVS = nullptr;
	ID3D11DomainShader* pDisplacementMappingDS = nullptr;
	ID3D11HullShader* pDisplacementMappingHS = nullptr;

	//displacement waves
	ID3D11VertexShader* pDisplacementWavesVS = nullptr;
	ID3D11PixelShader* pDisplacementWavesPS = nullptr;
	ID3D11HullShader* pDisplacementWavesHS = nullptr;
	ID3D11DomainShader* pDisplacementWavesDS = nullptr;

	//terrain with height map
	ID3D11VertexShader* pTerrainVS = nullptr;
	ID3D11InputLayout* pTerrainIL = nullptr;
	ID3D11HullShader* pTerrainHS = nullptr;
	ID3D11DomainShader* pTerrainDS = nullptr;
	ID3D11PixelShader* pTerrainPS = nullptr;

	//particles fire
	ID3D11InputLayout*    pSOIL = nullptr;
	ID3D11InputLayout*    pParticleDrawIL = nullptr;
	ID3D11VertexShader*   pSOVS = nullptr;
	ID3D11GeometryShader* pSOFireGS = nullptr;
	ID3D11VertexShader*   pParticleFireVS = nullptr;
	ID3D11GeometryShader* pParticleFireGS = nullptr;
	ID3D11PixelShader*    pParticleFirePS = nullptr;

	//particles rain
	ID3D11VertexShader* pParticleRainVS = nullptr;
	ID3D11GeometryShader* pParticleRainGS = nullptr;
	ID3D11GeometryShader* pParticleRainGSSO = nullptr;
	ID3D11PixelShader* pParticleRainPS = nullptr;

	//particles explosion
	ID3D11VertexShader* pParticleExplosionVS = nullptr;
	ID3D11GeometryShader* pParticleExplosionSOGS = nullptr;
	ID3D11GeometryShader* pParticleExplosionGS = nullptr;

	//particles fountain
	ID3D11VertexShader* pParticleFountainVS = nullptr;
	ID3D11GeometryShader* pParticleFountainSOGS = nullptr;
	ID3D11GeometryShader* pParticleFountainGS = nullptr;

	//ShadowMap
	ID3D11VertexShader* pShadowMapVS = nullptr;
	ID3D11PixelShader* pShadowMapPS = nullptr;

	//shadow map generation
	ID3D11VertexShader* pShadowMapGenVS = nullptr;
	ID3D11PixelShader* pShadowMapGenPS = nullptr;

	//shadowmap gen instanced
	ID3D11VertexShader* pShadowMapInstancedVS = nullptr;
	ID3D11InputLayout* pShadowMapInstancedIL = nullptr;
	ID3D11VertexShader* pShadowMapDrawInstancedVS = nullptr;

	//shadowMap skull
	ID3D11VertexShader* pSkullSMGenVS = nullptr;
	ID3D11VertexShader* pSkullSMDrawVS = nullptr;
	ID3D11PixelShader* pSkullSMPS = nullptr;
	ID3D11PixelShader* pSkullSMGenPS = nullptr;
	ID3D11InputLayout* pSkullSMIL = nullptr;

	//shadowMap plane
	ID3D11PixelShader* pPlaneShadowMapPS = nullptr;

};

