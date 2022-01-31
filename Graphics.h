#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include "dxgidebug.h"
#include "DirectXMath.h"
#include "directxcolors.h"
#include "CustomException.h"
#include "UtilityStructures.h"
#include "MathHelper.h"
#include "InputLayout.h"
#include "M3dLoader.h"
// #include "Shaders.h"
//////////////////////////////////////////////////////////////////////////
#include "DirectXTex/DirectXTex/DirectXTexP.h"
#include "DirectXTex/DirectXTex/DirectXTex.h"
#include "DirectXTex/DDSTextureLoader/DDSTextureLoader11.h"
//////////////////////////////////////////////////////////////////////////
#include <d3dcompiler.h>
#include <filesystem>
// #include <unordered_map>
#define MY_DEBUG
extern const short resolution_width;
extern const short resolution_height;


class Graphics
{
	struct M3dModel
	{
		ID3D11Buffer* pVertexBuffer = nullptr;
		ID3D11Buffer* pIndexBuffer = nullptr;
		std::vector<MaterialM3d> mats;
		std::vector<Subset> subsets;
	};
	struct M3dSkinnedModel
	{
		ID3D11Buffer* pVertexBuffer = nullptr;
		ID3D11Buffer* pIndexBuffer = nullptr;
		std::vector<MaterialM3d> mats;
		std::vector<Subset> subsets;
		std::vector<int> mBoneHierarchy;
		std::vector<DirectX::XMFLOAT4X4> mBoneOffsets;
	};
public:
	Graphics(HWND wnd);
	~Graphics();
	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;
	HWND GetWindowHandle() const noexcept;
	void SetViewport();
	void SetCommonShaderConstants(const DirectX::XMMATRIX& ViewProjection, const DirectX::XMMATRIX& View,
		const DirectX::XMMATRIX& Projection, const DirectX::XMMATRIX& lightViewProjection ,const DirectX::XMFLOAT3 camPos, float dt, float totalTime);
	void SetShadowTransform(const DirectX::XMMATRIX& shadowTransform);
	void UpdateLightDirection(const DirectX::XMFLOAT3& newLightDirection);
	void CreateCBuffers();
	template<typename CBuffer>
	void CreateRuntimeCBuffers(CBuffer& data, const std::string& name, const std::string& description);
	void CreateSRVs();
	void AddSRVToMap(const std::wstring& name, ID3D11ShaderResourceView* pSRV, bool diffuse, bool normal);
	void CreateAndBindSamplers();
	//techniques
	// Create normal map
	void ConstBufferNormalMapBind();
	void NormalMap(const DirectX::XMMATRIX world);
	void ReleaseNormalMapResource();
	//SSAO
	void ComputeSSAO(ID3D11RenderTargetView* pAmbientRTV0, D3D11_VIEWPORT& ssaoViewport,
		ID3D11ShaderResourceView* randomVecSRV,	ID3D11ShaderResourceView* pNormalMapSRV);
	void BlurSSAOMap(int blurCount, ID3D11RenderTargetView* pAmbientMapRTV0, ID3D11RenderTargetView* pAmbientMapRTV1,
		ID3D11ShaderResourceView* pAmbientMapSRV0, ID3D11ShaderResourceView* pAmbientMapSRV1, D3D11_VIEWPORT ssaoViewPort);
	void ReleaseSSAOShaderResource();

	void DefaultLightUpdate(MaterialEx& mat, BOOL disableTexSamling, BOOL useSSAO,
		const std::wstring& diffuseMap, const std::wstring& normalMap);
	void TerrainLightUpdate(MaterialEx& mat, BOOL disableTexSamling, BOOL useSSAO);
	void SetDefaultLightData();

	void ConstBufferShadowMapBind();
	void ShadowMap(const DirectX::XMMATRIX world, const DirectX::XMMATRIX& lightViewProj);

	void ConstBufferVSMatricesBind();
	void VSDefaultMatricesUpdate(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX texTransform, const DirectX::XMMATRIX& matTransform);

	//displacement waves
	void SetDispWavesShaderRes(const std::wstring& normalMap0, const std::wstring& normalMap1, const std::wstring& diffuseMap);
	void UpdateDispWavesCBuffers(const DirectX::XMMATRIX& world, MaterialEx& mat);

	//terrain
	void SetTerrainShaderResAndUpdateCbuffers(const DirectX::XMMATRIX world,
		const std::wstring blendMap, const std::wstring snowMap,
		ID3D11ShaderResourceView* pHeightMapDS,
		ID3D11ShaderResourceView* pHeightMapVS);
	//skybox is in 4th slot  of PS
	void BindCubeMap(std::wstring& skyBoxName) const;
	//
	//Compute shader waves
	//
	void SetComputeWavesResources();
	void UpdateComputeWaves(const DirectX::XMMATRIX& in_world);
	void DisturbComputeWaves(const UINT numColumns, const UINT numRows, DirectX::XMFLOAT3 waveContants);
	void UpdateSolutionComputeWaves(const UINT numColumns, const UINT numRows);
	void ComputeWavesClearVS();
	void SetWavesCSResources(ID3D11ShaderResourceView* prevSol, ID3D11ShaderResourceView* currSol, ID3D11ShaderResourceView* nextSol,
		ID3D11UnorderedAccessView* prevSolUA, ID3D11UnorderedAccessView* currSolUA, ID3D11UnorderedAccessView* nexSolUA);
	void SetComputeWavesSamplers();
	//
	//particles
	//
	void DrawParticle(DirectX::XMFLOAT3& emitPos, ParticlePick particle);
	void SetParticleBuffers(ID3D11Buffer* pStreamOutVB, ID3D11Buffer* pDrawVB, ID3D11ShaderResourceView* randomTexSRV, ID3D11Buffer* pInitVB,
		ParticlePick particle);

	void CreateM3dModel(M3dRawData& data, const std::string& name);
	void CreateM3dModel(M3dRawSkinnedData& data, const std::string& name);
	void DrawM3dStaticModel(std::string name, Technique tech, std::vector<DirectX::XMMATRIX> world);
private:
	void BindToSOStage(ID3D11Buffer* pStreamOutVB);
	void UnbindFromSOStage();
	void UpdateStreamOutConstBuffer(DirectX::XMFLOAT3& emitPos);
	void UpdateParticleDrawConstBuffer();
public:
	//buffers
	template <typename T>
	ID3D11Buffer* CreateVertexBuffer(const std::vector<T>& vertices, bool dynamic, bool streamOut, const std::wstring& name = std::wstring(),
		const UINT numMaxParticles = 1);
	ID3D11Buffer* CreateIndexBuffer(const std::vector<UINT> indices, const std::wstring& name = std::wstring());

	void CreateSRVArray(ID3D11ShaderResourceView** pSRV, UINT nImages, std::wstring* in_path);
#ifdef MY_DEBUG
public:
	void SetDebugName(ID3D11DeviceChild* child, const std::wstring& name);
	std::string wstrTostr(const std::wstring& wstr);
	void CheckFileExistence(Graphics& gfx, const std::wstring& path);
	void CheckFileExistence(Graphics* gfx, const std::wstring& path);
	void CheckFileExistence(const std::wstring& path);
private:
	void SetDeviceDebugName(ID3D11DeviceChild* child, const std::wstring& name);
#endif

public:
	Microsoft::WRL::ComPtr<ID3D11Device> pgfx_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pgfx_pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pgfx_RenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pgfx_DepthStencilView;
#ifdef MY_DEBUG
	ID3D11Debug* debugDevice = nullptr;
#endif
public:
	void BlurSSAOMap(ID3D11ShaderResourceView* pInputSRV, ID3D11RenderTargetView* pOutputRTV,
		D3D11_VIEWPORT ssaoViewPort, bool horizontalBlur);
private:
	template <typename CBData>
	ID3D11Buffer* CreateConstantBuffer(const CBData& data, bool dynamic, const std::string& name = std::string());

	ID3D11ShaderResourceView* CreateSRV(std::wstring& in_path, bool cubeMap);

	const CBufferNames cbNames;
	std::unordered_map<std::string, ID3D11Buffer*> constBuffersMap;
	//bind to register 0
	void BindDiffuseMap(const std::wstring& diffMapName) const;
	//bind to register 1
	void BindNormalMap(const std::wstring& normalMapName) const;

	//techniques
	//terrain
	void  ExtractFrustumPlanes(DirectX::XMFLOAT4 planes[6], DirectX::CXMMATRIX _M);


	std::unordered_map<std::wstring, ID3D11ShaderResourceView*> diffuseMaps;
	std::unordered_map<std::wstring, ID3D11ShaderResourceView**> diffuseMapArray;
	std::unordered_map<std::wstring, ID3D11ShaderResourceView*> normalMaps;
	std::unordered_map<std::wstring, ID3D11ShaderResourceView*> cubeMaps;
	std::unordered_map<std::string, M3dModel> m3dModelsMap;
	std::unordered_map<std::string, M3dSkinnedModel> m3dSkinnedModelMap;
	M3dModelNames m3dNames;
	std::vector<ID3D11SamplerState*> samplers;

	//common per frame updates
	DirectX::XMMATRIX mViewProjection;
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;
	DirectX::XMMATRIX mShadowTransform;
	DirectX::XMMATRIX mLightViewProjection;
	DirectX::XMFLOAT3 mCameraPosition;
	DirectX::XMFLOAT3 mNewLightDirection;
	
	const DirectX::XMFLOAT3 mDefaultLightDirection = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57335f);
	float mDeltaTime;
	float mTotalTime;

	//compute shader waves
	float time = 0.0f;
	DirectX::XMFLOAT2 waterTextureOffset;
	DirectX::XMMATRIX wavesOffset;
	const DirectX::XMMATRIX wavesScale = DirectX::XMMatrixScaling(5.0f, 5.0f, 1.0f);

	ID3D11UnorderedAccessView* pPreviousSolutionUAV = nullptr;
	ID3D11UnorderedAccessView* pCurrentSolutionUAV = nullptr;
	ID3D11UnorderedAccessView* pNextSolutionUAV = nullptr;

	ID3D11ShaderResourceView* pPreviousSolutionSRV = nullptr;
	ID3D11ShaderResourceView* pCurrentSolutionSRV = nullptr;
	ID3D11ShaderResourceView* pNextSolutionSRV = nullptr;


	//particles
	ID3D11Buffer* mStreamOutVBRain = nullptr;
	ID3D11Buffer* mDrawVBRain = nullptr;
	ID3D11Buffer* mInitVBRain = nullptr;
	ID3D11Buffer* mStreamOutVBExplosion = nullptr;
	ID3D11Buffer* mDrawVBExplosion = nullptr;
	ID3D11Buffer* mInitVBExplosion = nullptr;
	ID3D11Buffer* mStreamOutVBFountain = nullptr;
	ID3D11Buffer* mDrawVBFountain = nullptr;
	ID3D11Buffer* mInitVBFountain = nullptr;
	ID3D11Buffer* mStreamOutVBFire = nullptr;
	ID3D11Buffer* mDrawVBFire = nullptr;
	ID3D11Buffer* mInitVBFire = nullptr;
	float lastResetTime = 0.0f;
	bool mfirstRunRain = true;
	bool mfirstRunExplosion = true;
	bool mFirstRunFountain = true;
	bool mFirstRunFire = true;


	HWND windowHandle;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pgfx_SwapChain;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pgfx_BackBuffer;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pgfx_TextureDepthStencil;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDrawReflectionState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pMarkMirror;
	D3D11_VIEWPORT vp;
	D3D_FEATURE_LEVEL featureLevelIsSupported = {};
	UINT featureLevelNum = 7;
	const D3D_FEATURE_LEVEL d3dFeatureLevels [7]
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	// Transform NDC space [-1,+1]^2 to texture space [0,1]^2
	const DirectX::XMMATRIX toTexSpace = {
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f };

	void InitShaders();
public:
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
	void VS_IL_Init(ID3D11VertexShader** pVShader, const D3D11_INPUT_ELEMENT_DESC* inputLayout,
		ID3D11InputLayout** pIL, UINT nElements, const std::wstring& path);
	void VS_Init(ID3D11VertexShader** pVShader, const std::wstring& path);
	void PS_Init(ID3D11PixelShader** pPSShader, const std::wstring& path);
	void InitializeInputLayout(const D3D11_INPUT_ELEMENT_DESC* inputLayout, ID3D11InputLayout** pIL,
		UINT nElements, ID3DBlob* pBlob, const std::wstring& name);
	void GS_Init(ID3D11GeometryShader** pGSShader, const std::wstring& path);
	void GS_SO_Init(ID3D11GeometryShader** pGSShader, const std::wstring& path);
	void CS_Init(ID3D11ComputeShader** pCShader, const std::wstring& path);
	void HS_Init(ID3D11HullShader** pHShader, const std::wstring& path);
	void DS_Init(ID3D11DomainShader** pDshader, const std::wstring& path);

	InputLayout IL;
	ID3DBlob* pBlob = nullptr;
	Stream_Out SO;

	ID3D11InputLayout* pPositionIL = nullptr;

	//sky
	ID3D11VertexShader* pSkyVS = nullptr;
	ID3D11InputLayout* pSkyIL = nullptr;
	ID3D11PixelShader* pSkyPS = nullptr;
	//shadow map generation
	ID3D11VertexShader* pShadowMapGenVS = nullptr;
	ID3D11PixelShader* pShadowMapGenPS = nullptr;

	//compute SSAO
	ID3D11VertexShader* pSSAOFullScreenQuadVS = nullptr;
	ID3D11PixelShader* pSSAOFullScreenQuadPS = nullptr;
	//SSAO blur
	ID3D11VertexShader* pSSAOBlurVS = nullptr;
	ID3D11PixelShader* pSSAOBlurPS = nullptr;

	//debug quad
	ID3D11VertexShader* pDebugQuadVS = nullptr;
	ID3D11PixelShader* pDebugQuadPS = nullptr;

	ID3D11InputLayout* pPosNormalTexcTangentIL = nullptr;
	//Normal Map
	ID3D11VertexShader* pNormalMapVS = nullptr;
	ID3D11PixelShader* pNormalMapPS = nullptr;
	//ShadowMap
	ID3D11VertexShader* pShadowMapVS = nullptr;
	ID3D11PixelShader* pShadowMapPS = nullptr;

	ID3D11InputLayout* pPosNormalTexIL = nullptr;

	//default Light
	ID3D11VertexShader* pDefaultLightVS = nullptr;
	ID3D11PixelShader* pDefaultLightPS = nullptr;

	//Displacement waves
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

	//particles
	ID3D11VertexShader* pParticleStreamOutVS = nullptr;
	ID3D11InputLayout* pParticleStreamOutIL = nullptr;
	//particles fire
	ID3D11InputLayout* pParticleDrawIL = nullptr;
	ID3D11GeometryShader* pSOFireGS = nullptr;
	ID3D11VertexShader* pParticleFireVS = nullptr;
	ID3D11GeometryShader* pParticleFireGS = nullptr;
	ID3D11PixelShader* pParticleFirePS = nullptr;

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

	//compute shader waves
	ID3D11VertexShader* pComputeWavesVS = nullptr;
	ID3D11PixelShader* pComputeWavesPS = nullptr;
	ID3D11ComputeShader* pComputeWavesUpdateCS = nullptr;
	ID3D11ComputeShader* pComputeWavesDisturbCS = nullptr;

private:
	void InitializeRenderStates();
	void DestroyRenderStates();
public:
	//Rasterizer states
	ID3D11RasterizerState* WireframeRS;
	ID3D11RasterizerState* NoCullRS;
	ID3D11RasterizerState* CullClockwiseRS;
	ID3D11RasterizerState* CullCounterClockwiseRS;
	ID3D11RasterizerState* SolidFillRS;
	ID3D11RasterizerState* ShadowMapBiasRS;

	// Blend states
	ID3D11BlendState* AlphaToCoverageBS;
	ID3D11BlendState* TransparentBS;
	ID3D11BlendState* NoRenderTargetWritesBS;
	ID3D11BlendState* srsColor;
	ID3D11BlendState* additiveBlend;
	ID3D11BlendState* noBlendBS;

	// Depth/stencil states
	ID3D11DepthStencilState* MarkMirrorDSS;
	ID3D11DepthStencilState* DrawReflectionDSS;
	ID3D11DepthStencilState* NoDoubleBlendDSS;
	ID3D11DepthStencilState* DepthComplexityCountDSS;
	ID3D11DepthStencilState* DepthComplexityReadDSS;
	ID3D11DepthStencilState* LessEqualDSS;
	ID3D11DepthStencilState* disableDepthWrites;
	ID3D11DepthStencilState* EqualDSS;
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
};

template<typename CBuffer>
void Graphics::CreateRuntimeCBuffers(CBuffer& data, const std::string& name, const std::string& description)
{
	ID3D11Buffer* pBuffer = CreateConstantBuffer(data, false, description);
	constBuffersMap.insert(std::make_pair(name, pBuffer));
}

template <typename T>
ID3D11Buffer* Graphics::CreateVertexBuffer(const std::vector<T>& vertices, bool dynamic, bool streamOut, const std::wstring& name,
	const UINT numMaxParticles)
{
	D3D11_BUFFER_DESC desc;
	if (streamOut)
	{
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0u;
	}
	else if (dynamic)
	{
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.CPUAccessFlags = 0u;
	}
	desc.ByteWidth = static_cast<UINT>(sizeof(T) * vertices.size()) * numMaxParticles;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = vertices.data();
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;
	ID3D11Buffer* pBuffer = nullptr;
	HRESULT hr = pgfx_pDevice->CreateBuffer(&desc, &initData, &pBuffer);

	if (FAILED (hr))
	{
		std::wstring message = L"Failed Vertex Buffer creation of ";
		message += name;
		MessageBoxW(windowHandle, message.c_str(), NULL, MB_OK);
	}
	return pBuffer;
}

template <typename CBData>
ID3D11Buffer* Graphics::CreateConstantBuffer(const CBData& data, bool dynamic, const std::string& name)
{
	D3D11_BUFFER_DESC constBufDesc;
	if (dynamic)
		constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	else
		constBufDesc.CPUAccessFlags = 0u;
	constBufDesc.ByteWidth = sizeof(CBData);

	if (dynamic)
		constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	else
		constBufDesc.Usage = D3D11_USAGE_IMMUTABLE;
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.StructureByteStride = 0u;
	constBufDesc.MiscFlags = 0u;
	D3D11_SUBRESOURCE_DATA constBufInitData;
	constBufInitData.pSysMem = &data;
	constBufInitData.SysMemPitch = 0;
	constBufInitData.SysMemSlicePitch = 0;
	ID3D11Buffer* pBuffer = nullptr;
	HRESULT hr = pgfx_pDevice->CreateBuffer(&constBufDesc, &constBufInitData, &pBuffer);

	if (FAILED(hr))
	{
		std::string message = "Failed Constant Buffer creation of ";
		message += name;
		MessageBoxA(windowHandle, message.c_str(), NULL, MB_OK);
	}
	return pBuffer;
}

