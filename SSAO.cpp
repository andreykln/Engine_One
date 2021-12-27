#include "SSAO.h"

SSAO::SSAO(Graphics& gfx, UINT mWidth, UINT mHeight)
{
	BuildTextureViewsAndViewport(gfx, mWidth, mHeight);
	BuildRandomVectorTexture(gfx);
	BuildFullScreenQuadBuffers(gfx);
	BuildSamplers(gfx);
	BuildOffsetVectors();
	BuildFrustumFarCorners(0.5f * DirectX::XM_PI, 1000.0f);
	BuildConstantBuffer(gfx);

}

ID3D11ShaderResourceView* SSAO::GetNormalMapSRV()
{
	return pNormalMapSRV;
}

ID3D11ShaderResourceView* SSAO::GetRandomVectorSRV()
{
	return pRandomVectorsSRV;
}

void SSAO::ComputeSSAO(Graphics& gfx, DirectX::XMMATRIX viewM)
{
	// Bind the ambient map as the render target.  Observe that this pass does not bind 
	// a depth/stencil buffer--it does not need it, and without one, no depth test is
	// performed, which is what we want.
	ID3D11RenderTargetView* renderTargets[1] = { pAmbientRTV0 };
	gfx.pgfx_pDeviceContext->OMSetRenderTargets(1u, &renderTargets[0], 0u);
	gfx.pgfx_pDeviceContext->ClearRenderTargetView(pAmbientRTV0, DirectX::Colors::Black);
	gfx.pgfx_pDeviceContext->RSSetViewports(1u, &vp);

	UINT stride = sizeof(Vertex_IA);
	UINT offset = 0u;
	gfx.pgfx_pDeviceContext->IASetVertexBuffers(0u, 1u, &pQuadVertexBuffer, &stride, &offset);
	gfx.pgfx_pDeviceContext->IASetIndexBuffer(pQuadIndexBuffer, DXGI_FORMAT_R16_UINT, 0u);
	gfx.pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &pSSAOConstBuffer);
	gfx.pgfx_pDeviceContext->PSSetConstantBuffers(0u, 1u, &pSSAOConstBuffer);

	gfx.pgfx_pDeviceContext->PSSetShaderResources(0u, 1u, &pRandomVectorsSRV);
	gfx.pgfx_pDeviceContext->PSSetShaderResources(1u, 1u, &pNormalMapSRV);

	gfx.pgfx_pDeviceContext->PSSetSamplers(0u, 1u, &pNormalMapSampler);
	gfx.pgfx_pDeviceContext->PSSetSamplers(1u, 1u, &pRandomVectorSampler);


	UpdateSSAOConstBuffer(gfx, viewM);
	gfx.pgfx_pDeviceContext->DrawIndexed(6u, 0u, 0u);
}

void SSAO::SetNormalDepthRenderTarget(Graphics& gfx, ID3D11DepthStencilView* dsv)
{
	ID3D11RenderTargetView* renderTargets[1] = { pNormalMapTRV };
	gfx.pgfx_pDeviceContext->OMSetRenderTargets(1, renderTargets, dsv);
	// Clear view space normal to (0,0,-1) and clear depth to be very far away.  
	float clearColor[] = { 0.0f, 0.0f, -1.0f, 1e5f };
	gfx.pgfx_pDeviceContext->ClearRenderTargetView(pNormalMapTRV, clearColor);
}

void SSAO::BuildTextureViewsAndViewport(Graphics& gfx, UINT mWidth, UINT mHeight)
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = mWidth;
	texDesc.Height = mHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* normalDepthTexture = nullptr;
	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateTexture2D(&texDesc, 0u, &normalDepthTexture));
	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateShaderResourceView(normalDepthTexture, 0u, &pNormalMapSRV));
	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateRenderTargetView(normalDepthTexture, 0u, &pNormalMapTRV));
	normalDepthTexture->Release();

	ID3D11Texture2D* ambientTex0 = nullptr;
	texDesc.Width = mWidth / 2;
	texDesc.Height = mHeight / 2;
	texDesc.Format = DXGI_FORMAT_R16_FLOAT;
	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateTexture2D(&texDesc, 0u, &ambientTex0));
	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateShaderResourceView(ambientTex0, 0u, &pAmbientSRV0));
	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateRenderTargetView(ambientTex0, 0u, &pAmbientRTV0));

	ID3D11Texture2D* ambientTex1 = nullptr;
	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateTexture2D(&texDesc, 0u, &ambientTex1));
	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateShaderResourceView(ambientTex1, 0u, &pAmbientSRV1));
	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateRenderTargetView(ambientTex1, 0u, &pAmbientRTV1));

	ambientTex0->Release();
	ambientTex1->Release();


	vp.Width = mWidth / 2.0f;
	vp.Height = mHeight / 2.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

}

void SSAO::BuildFrustumFarCorners(float fovy, float farZ)
{
	float aspect = (float)resolution_width / (float)resolution_height;

	float halfHeight = farZ * tanf(0.5f * fovy);
	float halfWidth = aspect * halfHeight;

	frustumFarCorner[0] = DirectX::XMFLOAT4(-halfWidth, -halfHeight, farZ, 0.0f);
	frustumFarCorner[1] = DirectX::XMFLOAT4(-halfWidth, halfHeight, farZ, 0.0f);
	frustumFarCorner[2] = DirectX::XMFLOAT4(halfWidth, halfHeight, farZ, 0.0f);
	frustumFarCorner[3] = DirectX::XMFLOAT4(halfWidth, -halfHeight, farZ, 0.0f);
}

void SSAO::BuildOffsetVectors()
{
	// Start with 14 uniformly distributed vectors.  We choose the 8 corners of the cube
// and the 6 center points along each cube face.  We always alternate the points on 
// opposites sides of the cubes.  This way we still get the vectors spread out even
// if we choose to use less than 14 samples.

// 8 cube corners
	offsets[0] = XMFLOAT4(+1.0f, +1.0f, +1.0f, 0.0f);
	offsets[1] = XMFLOAT4(-1.0f, -1.0f, -1.0f, 0.0f);

	offsets[2] = XMFLOAT4(-1.0f, +1.0f, +1.0f, 0.0f);
	offsets[3] = XMFLOAT4(+1.0f, -1.0f, -1.0f, 0.0f);

	offsets[4] = XMFLOAT4(+1.0f, +1.0f, -1.0f, 0.0f);
	offsets[5] = XMFLOAT4(-1.0f, -1.0f, +1.0f, 0.0f);

	offsets[6] = XMFLOAT4(-1.0f, +1.0f, -1.0f, 0.0f);
	offsets[7] = XMFLOAT4(+1.0f, -1.0f, +1.0f, 0.0f);

	// 6 centers of cube faces
	offsets[8] = XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f);
	offsets[9] = XMFLOAT4(+1.0f, 0.0f, 0.0f, 0.0f);

	offsets[10] = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
	offsets[11] = XMFLOAT4(0.0f, +1.0f, 0.0f, 0.0f);

	offsets[12] = XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f);
	offsets[13] = XMFLOAT4(0.0f, 0.0f, +1.0f, 0.0f);

	for (int i = 0; i < 14; ++i)
	{
		// Create random lengths in [0.25, 1.0].
		float s = MathHelper::RandomFloatWithinRange(0.25f, 1.0f);

		XMVECTOR v = s * XMVector4Normalize(XMLoadFloat4(&offsets[i]));

		XMStoreFloat4(&offsets[i], v);
	}
}

void SSAO::BuildFullScreenQuadBuffers(Graphics& gfx)
{
	Vertex_IA v[4];

	v[0].pos = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f);
	v[1].pos = DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f);
	v[2].pos = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);
	v[3].pos = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f);

	// Store far plane frustum corner indices in Normal.x slot.
	v[0].normal = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	v[1].normal = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	v[2].normal = DirectX::XMFLOAT3(2.0f, 0.0f, 0.0f);
	v[3].normal = DirectX::XMFLOAT3(3.0f, 0.0f, 0.0f);

	v[0].tex = DirectX::XMFLOAT2(0.0f, 0.0f);
	v[1].tex = DirectX::XMFLOAT2(0.0f, 1.0f);
	v[2].tex = DirectX::XMFLOAT2(1.0f, 1.0f);
	v[3].tex = DirectX::XMFLOAT2(1.0f, 0.0f);

// 	v[0].tex = DirectX::XMFLOAT2(0.0f, 1.0f);
// 	v[1].tex = DirectX::XMFLOAT2(0.0f, 0.0f);
// 	v[2].tex = DirectX::XMFLOAT2(1.0f, 0.0f);
// 	v[3].tex = DirectX::XMFLOAT2(1.0f, 1.0f);


	D3D11_BUFFER_DESC buffDesc;
	buffDesc.ByteWidth = 4 * sizeof(Vertex_IA);
	buffDesc.Usage = D3D11_USAGE_IMMUTABLE;
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffDesc.CPUAccessFlags = 0u;
	buffDesc.MiscFlags = 0u;
	buffDesc.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA subresData;
	subresData.pSysMem = v;

	gfx.pgfx_pDevice->CreateBuffer(&buffDesc, &subresData, &pQuadVertexBuffer);

	USHORT indices[6] =
	{
		0, 1, 2,
		0, 2, 3
	};
	D3D11_BUFFER_DESC indexBuffDesc;
	indexBuffDesc.ByteWidth = 6 * sizeof(USHORT);
	indexBuffDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBuffDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBuffDesc.CPUAccessFlags = 0u;
	indexBuffDesc.MiscFlags = 0u;
	indexBuffDesc.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA subresData0;
	subresData0.pSysMem = indices;
	gfx.pgfx_pDevice->CreateBuffer(&indexBuffDesc, &subresData0, &pQuadIndexBuffer);
}

void SSAO::BuildSamplers(Graphics& gfx)
{
	D3D11_SAMPLER_DESC samplerRandomDesc;
	samplerRandomDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerRandomDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerRandomDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerRandomDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerRandomDesc.BorderColor[0] = 0.0f;
	samplerRandomDesc.BorderColor[1] = 0.0f;
	samplerRandomDesc.BorderColor[2] = 0.0f;
	samplerRandomDesc.BorderColor[3] = 0.0f;

	samplerRandomDesc.MipLODBias = 0.0f;
	samplerRandomDesc.MaxAnisotropy = 16;
	samplerRandomDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerRandomDesc.MinLOD = 0;
	samplerRandomDesc.MaxLOD = D3D11_FLOAT32_MAX;
	gfx.pgfx_pDevice->CreateSamplerState(&samplerRandomDesc, &pRandomVectorSampler);

	D3D11_SAMPLER_DESC samplerNormalMap;
	samplerNormalMap.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerNormalMap.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerNormalMap.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerNormalMap.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerNormalMap.BorderColor[0] = 0.0f;
	samplerNormalMap.BorderColor[1] = 0.0f;
	samplerNormalMap.BorderColor[2] = 0.0f;
	// Set a very far depth value if sampling outside of the NormalDepth map
	// so we do not get false occlusions.
	samplerNormalMap.BorderColor[3] = 1e5f;
	samplerNormalMap.MipLODBias = 0.0f;
	samplerNormalMap.MaxAnisotropy = 16;
	samplerNormalMap.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerNormalMap.MinLOD = 0;
	samplerNormalMap.MaxLOD = D3D11_FLOAT32_MAX;
	gfx.pgfx_pDevice->CreateSamplerState(&samplerNormalMap, &pNormalMapSampler);
}

void SSAO::BuildConstantBuffer(Graphics& gfx)
{
	SSAOConstBuffer cBuff;
	for (int i = 0; i < 14; i++)
	{
		cBuff.offsetVectors[i] = DirectX::XMLoadFloat4(&offsets[i]);
	}
	for (int i = 0; i < 4; i++)
	{
		cBuff.frustumFarCorners[i] = DirectX::XMLoadFloat4(&frustumFarCorner[i]);
	}

	D3D11_BUFFER_DESC buffDesc;
	buffDesc.ByteWidth = sizeof(SSAOConstBuffer);
	buffDesc.Usage = D3D11_USAGE_DYNAMIC;
	buffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffDesc.MiscFlags = 0u;
	buffDesc.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA subresData;
	subresData.pSysMem = &cBuff;
	gfx.pgfx_pDevice->CreateBuffer(&buffDesc, &subresData, &pSSAOConstBuffer);
}

void SSAO::BuildRandomVectorTexture(Graphics& gfx)
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.Width = 256;
	texDesc.Height = 256;
	texDesc.ArraySize = 1;
	texDesc.MipLevels = 1;
	texDesc.SampleDesc.Count = 1u;
	texDesc.SampleDesc.Quality = 0u;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0u;
	texDesc.MiscFlags = 0u;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;

	const int size = 256 * 256;

	DirectX::PackedVector::XMCOLOR* col = new DirectX::PackedVector::XMCOLOR[size];

	for (int i = 0; i < 256; ++i)
	{
		for (int j = 0; j < 256; ++j)
		{
			XMFLOAT3 v(MathHelper::RandomFloatWithinRange(0.0f, 1.0f),
					   MathHelper::RandomFloatWithinRange(0.0f, 1.0f),
					   MathHelper::RandomFloatWithinRange(0.0f, 1.0f));

			col[i * 256 + j] = DirectX::PackedVector::XMCOLOR(v.x, v.y, v.z, 0.0f);
		}
	}

	D3D11_SUBRESOURCE_DATA subresData;
	subresData.SysMemPitch = 256 * sizeof(DirectX::PackedVector::XMCOLOR);
	subresData.pSysMem = col;

	ID3D11Texture2D* pTex = nullptr;
	gfx.pgfx_pDevice->CreateTexture2D(&texDesc, &subresData, &pTex);
	gfx.pgfx_pDevice->CreateShaderResourceView(pTex, 0u, &pRandomVectorsSRV);

	delete[] col;
	col = nullptr;

	pTex->Release();
}

void SSAO::UpdateSSAOConstBuffer(Graphics& gfx, DirectX::XMMATRIX mView)
{
	// Transform NDC space [-1,+1]^2 to texture space [0,1]^2
	static const XMMATRIX T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);
	viewToTextureSpace = DirectX::XMMatrixMultiply(mView, T);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pSSAOConstBuffer, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	SSAOConstBuffer* pBuffer = reinterpret_cast<SSAOConstBuffer*>(mappedData.pData);
	pBuffer->viewToTexSpace = DirectX::XMMatrixTranspose(viewToTextureSpace);
	gfx.pgfx_pDeviceContext->Unmap(pSSAOConstBuffer, 0u);
}
