#include "SSAO.h"

SSAO::SSAO(Graphics& gfx, UINT mWidth, UINT mHeight)
{
	BuildTextureViewsAndViewport(gfx, mWidth, mHeight);
	BuildRandomVectorTexture(gfx);
	BuildFullScreenQuadBuffers(gfx);
	BuildOffsetVectors();
	BuildFrustumFarCorners(FOV_max, 1000.0f);

}

ID3D11Buffer** SSAO::GetQuadVertexBuffer()
{
	return &pQuadVertexBuffer;
}

ID3D11Buffer* SSAO::GetQuadIndexBuffer()
{
	return pQuadIndexBuffer;
}

UINT SSAO::GetQuadIndexCount()
{
	return quadIndexCount;
}

ID3D11ShaderResourceView* SSAO::GetNormalMapSRV()
{
	return pNormalMapSRV;
}

ID3D11ShaderResourceView* SSAO::GetRandomVectorSRV()
{
	return pRandomVectorsSRV;
}

D3D11_VIEWPORT& SSAO::GetSSAOViewport()
{
	return vp;
}

ID3D11RenderTargetView* SSAO::GetAmbientMapRTV0()
{
	return pAmbientRTV0;
}

ID3D11RenderTargetView* SSAO::GetAmbientMapRTV1()
{
	return pAmbientRTV1;
}

ID3D11ShaderResourceView* SSAO::GetAmbientMapSRV0()
{
	return pAmbientSRV0;
}

ID3D11ShaderResourceView* SSAO::GetAmbientMapSRV1()
{
	return pAmbientSRV1;
}

void SSAO::SetNormalDepthRenderTarget(Graphics& gfx, ID3D11DepthStencilView* dsv)
{
	gfx.pgfx_pDeviceContext->ClearDepthStencilView(
		gfx.pgfx_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	gfx.SetViewport();
	ID3D11RenderTargetView* renderTargets[1] = { pNormalMapRTV };
	gfx.pgfx_pDeviceContext->OMSetRenderTargets(1, renderTargets, dsv);
	// Clear view space normal to (0,0,-1) and clear depth to be very far away.  
	//RenderDoc yells at 1e5f interpreting it is INF, but maybe this is OK
	float clearColor[] = { 0.0f, 0.0f, -1.0f, 1e5f };
	gfx.pgfx_pDeviceContext->ClearRenderTargetView(pNormalMapRTV, clearColor);
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
	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateRenderTargetView(normalDepthTexture, 0u, &pNormalMapRTV));
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
	using namespace DirectX;
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
	std::vector<vbPosNormalTex> vertices(4);

	vertices[0].pos = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[1].pos = DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f);
	vertices[2].pos = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertices[3].pos = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f);

	// Store far plane frustum corner indices in Normal.x slot.
	vertices[0].normal = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertices[1].normal = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	vertices[2].normal = DirectX::XMFLOAT3(2.0f, 0.0f, 0.0f);
	vertices[3].normal = DirectX::XMFLOAT3(3.0f, 0.0f, 0.0f);

	vertices[0].tex = DirectX::XMFLOAT2(0.0f, 1.0f);
	vertices[1].tex = DirectX::XMFLOAT2(0.0f, 0.0f);
	vertices[2].tex = DirectX::XMFLOAT2(1.0f, 0.0f);
	vertices[3].tex = DirectX::XMFLOAT2(1.0f, 1.0f);
	pQuadVertexBuffer = gfx.CreateVertexBuffer(vertices, false, false, L"ssao quad");
	std::vector<UINT> indices = 
	{
		0, 1, 2,
		0, 2, 3
	};
	pQuadIndexBuffer = gfx.CreateIndexBuffer(indices, L"SSAO quad");
	quadIndexCount = static_cast<UINT>(indices.size());
}


cbComputeSSAOconstData SSAO::GetAndBuildConstantBufferData()
{
	for (int i = 0; i < 14; i++)
	{
		computeSSAOData.offsetVectors[i] = DirectX::XMLoadFloat4(&offsets[i]);
	}
	for (int i = 0; i < 4; i++)
	{
		computeSSAOData.frustumFarCorners[i] = frustumFarCorner[i];
	}
	return computeSSAOData;
}

cbComputeSSAO& SSAO::GetConstBufferSSAODataPerFrame()
{
	return computeSSAOcbuff;
}

void SSAO::BuildRandomVectorTexture(Graphics& gfx)
{
	using namespace DirectX;

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

