#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include "dxgidebug.h"
#include "directxmath.h"
#include <d3dcompiler.h>
#include "CustomException.h"


class Graphics
{
public:
	
	Graphics(HWND wnd);
	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;
	void TestDrawing()
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 Pos;
			DirectX::XMFLOAT4 Color;
		};
		const Vertex cubeCoord[]
		{
			DirectX::XMFLOAT3 {-1.0f, -1.0f, 0.0f},
			DirectX::XMFLOAT4 {1.0f, 0.0f, 0.0f,1.0f},
			DirectX::XMFLOAT3 {1.0f, -1.0f, 0.0f},
			DirectX::XMFLOAT4 {0.0f, 1.0f, 0.0f,1.0f},
			DirectX::XMFLOAT3 {0.0f, 1.0f, 0.0f},
			DirectX::XMFLOAT4 {0.0f, 0.0f, 1.0f,1.0f},
		};

		ID3D11InputLayout* pInputLayout;
		const D3D11_INPUT_ELEMENT_DESC inputElemDesc[] =
		{
			{"Position", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA, 0u},
			{"Color", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA, 0u}
		};



		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(Vertex) * 6;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0u;
		// Fill in the sub resource data.
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = cubeCoord;
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;
		Microsoft::WRL::ComPtr <ID3D11Buffer> pVertexBuffer;


		ID3D11VertexShader* pVertexShader;
		DX::ThrowIfFailed(pgfx_pDevice->CreateBuffer(&bufferDesc, &initData, pVertexBuffer.GetAddressOf()));
		ID3DBlob* pBlob = {};
		D3DReadFileToBlob((LPCWSTR)"VertexShader.hlsl", &pBlob);
		pgfx_pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr,&pVertexShader);

		pgfx_pDevice->CreateInputLayout(inputElemDesc, 2u,pBlob->GetBufferPointer() ,sizeof(pBlob), &pInputLayout);




		const UINT indices[3]
		{
			0,1,2
		};

		D3D11_BUFFER_DESC indicesBuffDesc;
		indicesBuffDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indicesBuffDesc.ByteWidth = sizeof(UINT) * 3;
		indicesBuffDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indicesBuffDesc.CPUAccessFlags = 0u;
		indicesBuffDesc.MiscFlags = 0u;
		indicesBuffDesc.StructureByteStride = 0u;
		D3D11_SUBRESOURCE_DATA indicesInitData;
		indicesInitData.pSysMem = indices;
		indicesInitData.SysMemPitch = 0u;
		indicesInitData.SysMemSlicePitch = 0u;
		ID3D11Buffer* pIndicesBuffer;
		DX::ThrowIfFailed(pgfx_pDevice->CreateBuffer(&indicesBuffDesc, &indicesInitData, &pIndicesBuffer));
		pgfx_pDeviceContext->IASetIndexBuffer(pIndicesBuffer, DXGI_FORMAT_R32_UINT, 0u);
		

		const UINT stride = sizeof(Vertex);
		const UINT offset = 0;
		pgfx_pDeviceContext->IASetVertexBuffers(0u, 1u, &pVertexBuffer, &stride, &offset);

	
		
		
	


		pgfx_pDeviceContext->DrawIndexed()


	}

private:
	Microsoft::WRL::ComPtr<ID3D11Device> pgfx_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pgfx_pDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pgfx_SwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pgfx_RenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pgfx_BackBuffer;

	D3D_FEATURE_LEVEL featureLevelIsSupported;
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
};

