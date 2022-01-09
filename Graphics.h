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
	void SetMatrices(const DirectX::XMMATRIX& ViewProjection, const DirectX::XMMATRIX& View, const DirectX::XMMATRIX& Projection);

	void CreateCBuffers();
	void CreateRuntimeCBuffers(cbComputeSSAO& ssauBuffer);
	void CreateSRVs();
	void CreateAndBindSamplers();
	//techniques
	void ConstBufferNormalMapBind();
	void NormalMap(const DirectX::XMMATRIX world);
	void ReleaseNormalMapResource();
	void ComputeSSAO(ID3D11RenderTargetView* pAmbientRTV0, D3D11_VIEWPORT& ssaoViewport,
		ID3D11ShaderResourceView* randomVecSRV,	ID3D11ShaderResourceView* pNormalMapSRV);

	void ConstBufferShadowMapBind();
	void ShadowMap(const DirectX::XMMATRIX world, const DirectX::XMMATRIX& lightViewProj);

	void ConstBufferVSMatricesBind();
	void VSDefaultMatricesUpdate(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX texTransform,
		const DirectX::XMMATRIX& shadowTransform, const DirectX::XMMATRIX& matTransform,
		const DirectX::XMFLOAT3& cameraPositon);

	//skybox is in third slot of PS
	void BindCubeMap(std::wstring& skyBoxName) const;
	void BindDiffuseMap(std::wstring& diffMapName) const;
	void BindNormalMap(std::wstring& normalMapName) const;
	//buffers
	template <typename T>
	ID3D11Buffer* CreateVertexBuffer(const std::vector<T>& vertices, bool dynamic, bool streamOut, const std::wstring& name = std::wstring());
	ID3D11Buffer* CreateIndexBuffer(const std::vector<UINT> indices, const std::wstring& name = std::wstring());

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
	//byteWidth needed because sizeof(CBData) is giving wrong number for some reason.
	template <typename CBData>
	ID3D11Buffer* CreateConstantBuffer(const CBData& data, const UINT byteWidth, const std::wstring& name = std::wstring());
	ID3D11ShaderResourceView* CreateSRV(std::wstring& in_path, bool cubeMap);
	std::unordered_map<std::string, ID3D11Buffer*> constBuffersMap;
	//contains indices for corresponding texture in texture array
	std::unordered_map<std::wstring, ID3D11ShaderResourceView*> diffuseMaps;
	std::unordered_map<std::wstring, ID3D11ShaderResourceView*> normalMaps;
	///
	std::unordered_map<std::wstring, ID3D11ShaderResourceView*> cubeMaps;
	std::vector<ID3D11SamplerState*> samplers;

	DirectX::XMMATRIX mViewProjection;
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;
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
ID3D11Buffer* Graphics::CreateConstantBuffer(const CBData& data, const UINT byteWidth, const std::wstring& name /*= std::wstring()*/)
{
	D3D11_BUFFER_DESC constBufDesc;
	constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufDesc.ByteWidth = byteWidth;
	constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
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
		std::wstring message = L"Failed Constant Buffer creation of ";
		message += name;
		MessageBoxW(windowHandle, message.c_str(), NULL, MB_OK);
	}
	return pBuffer;
}

