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
// #include "ShaderResourceView.h"
//////////////////////////////////////////////////////////////////////////
#include "DirectXTex/DirectXTex/DirectXTexP.h"
#include "DirectXTex/DirectXTex/DirectXTex.h"
#include "DirectXTex/DDSTextureLoader/DDSTextureLoader11.h"
//////////////////////////////////////////////////////////////////////////
#include <d3dcompiler.h>
#include <filesystem>
#include <unordered_map>
#define MY_DEBUG
extern const short resolution_width;
extern const short resolution_height;

class Graphics
{
public:
	Graphics(HWND wnd);
	~Graphics();
	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;
	void DrawIndexed(UINT count) const noexcept;
	void DrawIndexed(UINT count, UINT startIndexLocation, UINT startVertexLocation);
	void Draw(UINT VertexCount, UINT StartVertexLocation) const noexcept;
	void DrawIndexedTwo(UINT count, UINT StartIndexLocation, INT BaseVertexLocation) const noexcept;
	void DrawInstancedIndexed(UINT count, UINT instanceCount, UINT startIndexLocation, int baseVertexLocation, UINT startInstanceLocation);
	HWND GetWindowHandle() const noexcept;
	void SetViewport();
	void SetCommonShaderConstants(const DirectX::XMMATRIX& ViewProjection, const DirectX::XMMATRIX& View,
		const DirectX::XMMATRIX& Projection, const DirectX::XMFLOAT3 camPos, float dt);
	void SetShadowTransform(const DirectX::XMMATRIX& shadowTransform);
	void UpdateLightDirection(const DirectX::XMFLOAT3& newLightDirection);
	void CreateCBuffers();
	template<typename CBuffer>
	void CreateRuntimeCBuffers(CBuffer& data, const std::string& name, const std::string& description);
	void CreateSRVs();
	void AddSRVToMap(const std::wstring& name, ID3D11ShaderResourceView* pSRV, bool diffuse, bool normal);
	void CreateAndBindSamplers();
	//techniques
	void ConstBufferNormalMapBind();
	void NormalMap(const DirectX::XMMATRIX world);
	void ReleaseNormalMapResource();
	void ComputeSSAO(ID3D11RenderTargetView* pAmbientRTV0, D3D11_VIEWPORT& ssaoViewport,
		ID3D11ShaderResourceView* randomVecSRV,	ID3D11ShaderResourceView* pNormalMapSRV);
	void BlurSSAOMap(int blurCount, ID3D11RenderTargetView* pAmbientMapRTV0, ID3D11RenderTargetView* pAmbientMapRTV1,
		ID3D11ShaderResourceView* pAmbientMapSRV0, ID3D11ShaderResourceView* pAmbientMapSRV1, D3D11_VIEWPORT ssaoViewPort);
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
	//buffers
	template <typename T>
	ID3D11Buffer* CreateVertexBuffer(const std::vector<T>& vertices, bool dynamic, bool streamOut, const std::wstring& name = std::wstring());
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
private:
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
	std::vector<ID3D11SamplerState*> samplers;

	//common per frame updates
	DirectX::XMMATRIX mViewProjection;
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;
	DirectX::XMMATRIX mShadowTransform;
	DirectX::XMFLOAT3 mCameraPosition;
	DirectX::XMFLOAT3 mNewLightDirection;
	const DirectX::XMFLOAT3 mDefaultLightDirection = DirectX::XMFLOAT3(0.57735f, -0.57735, 0.57335);
	float deltaTime;

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

};

template<typename CBuffer>
void Graphics::CreateRuntimeCBuffers(CBuffer& data, const std::string& name, const std::string& description)
{
	ID3D11Buffer* pBuffer = CreateConstantBuffer(data, false, description);
	constBuffersMap.insert(std::make_pair(name, pBuffer));

}

template <typename T>
ID3D11Buffer* Graphics::CreateVertexBuffer(const std::vector<T>& vertices, bool dynamic, bool streamOut, const std::wstring& name)
{
	D3D11_BUFFER_DESC desc;
	if (streamOut)
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
	else
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	if (dynamic)
	{
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.CPUAccessFlags = 0u;
	}
	desc.ByteWidth = static_cast<UINT>(sizeof(T) * vertices.size());//size of the structure
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0; //only for structured buffer
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

