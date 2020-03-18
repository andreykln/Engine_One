#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include "dxgidebug.h"
#include "DirectXMath.h"
#include "directxmath.h"
#include "directxcolors.h"
#include <d3dcompiler.h>
#include "CustomException.h"

extern const short resolution_width;
extern const short resolution_height;

class Graphics
{
public:
	//TODO  put all of your render state group objects in a static class
	Graphics(HWND wnd);
	
	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;
	void TestDrawing(float angle, float axis_x, float axis_y, float axis_z)
	{
		

		const float FOV = DirectX::XM_PI / 4.0f;
		const float screenAspect = float(resolution_width) / float(resolution_height);
		DirectX::XMMATRIX projectionMatrix;
		projectionMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationX(angle)*
			DirectX::XMMatrixRotationZ(angle) *
			DirectX::XMMatrixTranslation(-axis_x, -axis_y, axis_z) *
			DirectX::XMMatrixPerspectiveFovLH(FOV,screenAspect, 0.1f, 100.0f));
		
		DirectX::XMFLOAT4X4 vs_WorldViewProjection;
		DirectX::XMStoreFloat4x4(&vs_WorldViewProjection, projectionMatrix);

		D3D11_BUFFER_DESC constBufDesc;
		constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constBufDesc.ByteWidth = sizeof(vs_WorldViewProjection);
		constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
		constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constBufDesc.StructureByteStride = sizeof(DirectX::XMFLOAT4X4);
		constBufDesc.MiscFlags = 0u;
		D3D11_SUBRESOURCE_DATA constBufInitData;
		constBufInitData.pSysMem = &vs_WorldViewProjection;
		constBufInitData.SysMemPitch = 0;
		constBufInitData.SysMemSlicePitch = 0;

		Microsoft::WRL::ComPtr<ID3D11Buffer> pConstBuffer;

		DX::ThrowIfFailed(pgfx_pDevice->CreateBuffer(&constBufDesc, &constBufInitData, pConstBuffer.ReleaseAndGetAddressOf()));
		pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, pConstBuffer.GetAddressOf());

		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT4 color;
		};
		const Vertex cubeCoord[]
		{


			{DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f),		DirectX::XMFLOAT4(DirectX::Colors::BurlyWood)} ,
			{DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f),		DirectX::XMFLOAT4(DirectX::Colors::AliceBlue)} ,
			{DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f),		DirectX::XMFLOAT4(DirectX::Colors::Beige)} ,

			{DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f),		DirectX::XMFLOAT4(DirectX::Colors::Cyan)} ,
			{DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f),		DirectX::XMFLOAT4(DirectX::Colors::Honeydew)} ,
			{DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f),		DirectX::XMFLOAT4(DirectX::Colors::LightGray)} ,

			{DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f),		DirectX::XMFLOAT4(DirectX::Colors::OliveDrab)} ,
			{DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f),		DirectX::XMFLOAT4(DirectX::Colors::WhiteSmoke)} ,

		};
		Microsoft::WRL::ComPtr <ID3D11Buffer> pVertexBuffer;

		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(cubeCoord);//size of the structure
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = sizeof(Vertex); //size of the each element in the structure
		// Fill in the sub resource data.
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = cubeCoord;
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;
		const UINT stride = sizeof(Vertex);
		const UINT offset = 0;
		DX::ThrowIfFailed(pgfx_pDevice->CreateBuffer(&bufferDesc, &initData, pVertexBuffer.ReleaseAndGetAddressOf()));
		pgfx_pDeviceContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset); 


		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
		Microsoft::WRL::ComPtr<ID3DBlob>pVertexShaderBlob;
		DX::ThrowIfFailed(D3DReadFileToBlob(L"VertexShader.cso", &pVertexShaderBlob));
		DX::ThrowIfFailed(pgfx_pDevice->CreateVertexShader( pVertexShaderBlob->GetBufferPointer(),
															pVertexShaderBlob->GetBufferSize(),
															nullptr, &pVertexShader));
		pgfx_pDeviceContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
		Microsoft::WRL::ComPtr<ID3DBlob> pPixelShaderBlob;

		DX::ThrowIfFailed(D3DReadFileToBlob(L"PixelShader.cso", &pPixelShaderBlob)); //TODO what is the difference between GetBuffersize and Sizeof
		DX::ThrowIfFailed(pgfx_pDevice->CreatePixelShader(pPixelShaderBlob->GetBufferPointer(), pPixelShaderBlob->GetBufferSize(), nullptr, &pPixelShader));
		pgfx_pDeviceContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

		Microsoft::WRL::ComPtr< ID3D11InputLayout> pInputLayout;
		const D3D11_INPUT_ELEMENT_DESC inputElemDesc[] =
		{
			{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA, 0u},
			{"Color", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA, 0u}
		};


		DX::ThrowIfFailed(pgfx_pDevice->CreateInputLayout(inputElemDesc,
			(UINT)std::size(inputElemDesc), pVertexShaderBlob->GetBufferPointer(), 
			pVertexShaderBlob->GetBufferSize(), &pInputLayout));

		pgfx_pDeviceContext->IASetInputLayout(pInputLayout.Get());




		const unsigned short  indices[]
		{

			//front
			0,2,1, 0,3,2,
			//back
			7,5,6, 6,5,4,
			//right side
			1,2,7, 2,5,7,
			//left side
			6,3,0, 6,4,3,
			//up
			2,4,5, 3,4,2,
			//down
			1,7,6, 1,6,0
		};

		D3D11_BUFFER_DESC indicesBuffDesc;
		indicesBuffDesc.Usage = D3D11_USAGE_DEFAULT;
		indicesBuffDesc.ByteWidth = sizeof(indices);
		indicesBuffDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indicesBuffDesc.CPUAccessFlags = 0u;
		indicesBuffDesc.MiscFlags = 0u;
		indicesBuffDesc.StructureByteStride = sizeof(unsigned short);
		D3D11_SUBRESOURCE_DATA indicesInitData;
		indicesInitData.pSysMem = indices;
		indicesInitData.SysMemPitch = 0u;
		indicesInitData.SysMemSlicePitch = 0u;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pIndicesBuffer;
		DX::ThrowIfFailed(pgfx_pDevice->CreateBuffer(&indicesBuffDesc, &indicesInitData, &pIndicesBuffer));
		pgfx_pDeviceContext->IASetIndexBuffer(pIndicesBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
		

		pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		//pgfx_pDeviceContext->OMSetRenderTargets(1u, pgfx_RenderTargetView.GetAddressOf(), nullptr);
		
		


		pgfx_pDeviceContext->DrawIndexed((UINT)std::size(indices), 0, 0);


	}

private:
	D3D11_VIEWPORT vp;

	Microsoft::WRL::ComPtr<ID3D11Device> pgfx_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pgfx_pDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pgfx_SwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pgfx_RenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pgfx_BackBuffer;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pgfx_TextureDepthStencil;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pgfx_DepthStencilView;

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


};

