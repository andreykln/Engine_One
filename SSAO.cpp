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

ID3D11RenderTargetView* SSAO::GetAmbientRTV()
{
	return pAmbientRTV0;
}

D3D11_VIEWPORT& SSAO::GetSSAOViewport()
{
	return vp;
}


void SSAO::ComputeSSAO(Graphics& gfx, DirectX::XMMATRIX mProj)
{
// 	ID3D11ShaderResourceView* pNULLSRV = nullptr;
// 	//clear previous frame's binding
// 	gfx.pgfx_pDeviceContext->PSSetShaderResources(4u, 1u, &pNULLSRV);
// 
// 
// 	// Bind the ambient map as the render target.  Observe that this pass does not bind 
// 	// a depth/stencil buffer--it does not need it, and without one, no depth test is
// 	// performed, which is what we want.
// 	ID3D11RenderTargetView* renderTargets[1] = { pAmbientRTV0 };
// 	gfx.pgfx_pDeviceContext->OMSetRenderTargets(1u, &renderTargets[0], 0u);
// 	gfx.pgfx_pDeviceContext->ClearRenderTargetView(pAmbientRTV0, DirectX::Colors::Black);
// 	gfx.pgfx_pDeviceContext->RSSetViewports(1u, &vp);
// 
// 	UINT stride = sizeof(Vertex_IA);
// 	UINT offset = 0u;
// 	gfx.pgfx_pDeviceContext->IASetVertexBuffers(0u, 1u, &pQuadVertexBuffer, &stride, &offset);
// 	gfx.pgfx_pDeviceContext->IASetIndexBuffer(pQuadIndexBuffer, DXGI_FORMAT_R16_UINT, 0u);
// 	gfx.pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &pSSAOConstBuffer);
// 	gfx.pgfx_pDeviceContext->PSSetConstantBuffers(0u, 1u, &pSSAOConstBuffer);
// 
// 	gfx.pgfx_pDeviceContext->PSSetShaderResources(0u, 1u, &pRandomVectorsSRV);
// 	gfx.pgfx_pDeviceContext->PSSetShaderResources(1u, 1u, &pNormalMapSRV);


	UpdateSSAOConstBuffer(gfx, mProj);
	gfx.pgfx_pDeviceContext->DrawIndexed(6u, 0u, 0u);
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


void SSAO::DrawDebugScreenQuad(Graphics& gfx, Shaders* shaders)
{
	shaders->BindVSandIA(DrawDebugTexQuad_VS_PS);
	shaders->BindPS(DrawDebugTexQuad_VS_PS);
	gfx.pgfx_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	UINT stride = sizeof(Vertex_IA);
	UINT offset = 0u;
	gfx.pgfx_pDeviceContext->IASetVertexBuffers(0u, 1u, &pQuadVertexBuffer, &stride, &offset);
	gfx.pgfx_pDeviceContext->IASetIndexBuffer(pQuadIndexBuffer, DXGI_FORMAT_R16_UINT, 0u);

	//pNormalMapSRV pAmbientSRV0 pAmbientSRV1
	gfx.pgfx_pDeviceContext->PSSetShaderResources(0u, 1u, &pAmbientSRV1);

// 	gfx.pgfx_pDeviceContext->PSSetSamplers(0u, 1u, &pRandomVectorSampler);
	gfx.pgfx_pDeviceContext->DrawIndexed(6, 0u, 0u);
	shaders->UnbindAll();
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
	quadIndexCount = indices.size();
}



void SSAO::BlurAmbientMap(Graphics& gfx, int blurCount, Shaders* pShader)
{

	pShader->BindVSandIA(SSAOBlur_VS_PS);
	pShader->BindPS(SSAOBlur_VS_PS);
	gfx.pgfx_pDeviceContext->PSSetShaderResources(0u, 1u, &pNormalMapSRV);
	for (int i = 0; i < blurCount; i++)
	{
		// Ping-pong the two ambient map textures as we apply
		// horizontal and vertical blur passes.
		BlurAmbientMap(gfx, pAmbientSRV0, pAmbientRTV1, true);
		BlurAmbientMap(gfx, pAmbientSRV1, pAmbientRTV0, false);

	}
	pShader->UnbindVS();
	pShader->UnbindPS();
}

void SSAO::BlurAmbientMap(Graphics& gfx, ID3D11ShaderResourceView* pInputSRV, ID3D11RenderTargetView* pOutputRTV, bool horizontalBlur)
{
	ID3D11RenderTargetView* renderTargets[1] = { pOutputRTV };
	gfx.pgfx_pDeviceContext->OMSetRenderTargets(1, &renderTargets[0], 0);
	gfx.pgfx_pDeviceContext->ClearRenderTargetView(renderTargets[0], DirectX::Colors::Black);
	gfx.pgfx_pDeviceContext->RSSetViewports(1u, &vp);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pSSAOBlurBuffer, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	SSAOBlur* pBuffer = reinterpret_cast<SSAOBlur*>(mappedData.pData);
	if (horizontalBlur)
	{
		pBuffer->horizBool = true;
	}
	else
	{
		pBuffer->horizBool = false;
	}
	pBuffer->texelHeight = 1.0f / vp.Height;
	pBuffer->texelWidth = 1.0f / vp.Width;
	gfx.pgfx_pDeviceContext->Unmap(pSSAOBlurBuffer, 0u);

	gfx.pgfx_pDeviceContext->PSSetConstantBuffers(0u, 1u, &pSSAOBlurBuffer);
	gfx.pgfx_pDeviceContext->PSSetShaderResources(1u, 1u, &pInputSRV);

	UINT stride = sizeof(Vertex_IA);
	UINT offset = 0u;
	gfx.pgfx_pDeviceContext->IASetVertexBuffers(0u, 1u, &pQuadVertexBuffer, &stride, &offset);
	gfx.pgfx_pDeviceContext->IASetIndexBuffer(pQuadIndexBuffer, DXGI_FORMAT_R16_UINT, 0u);
	gfx.pgfx_pDeviceContext->DrawIndexed(6, 0u, 0u);
	ID3D11ShaderResourceView* pNULLSRV = nullptr;
	gfx.pgfx_pDeviceContext->PSSetShaderResources(1u, 1u, &pNULLSRV);
	ID3D11RenderTargetView* pNULLRTV = nullptr;
	renderTargets[0] = pNULLRTV;
	gfx.pgfx_pDeviceContext->OMSetRenderTargets(1, &renderTargets[0], 0);

}

void SSAO::SetSSAOMapToPS(Graphics& gfx)
{
	gfx.pgfx_pDeviceContext->PSSetShaderResources(4u, 1u, &pAmbientSRV0);
}

cbComputeSSAO& SSAO::GetAndBuildConstantBufferData()
{
	for (int i = 0; i < 14; i++)
	{
		computeSSAOcbuff.offsetVectors[i] = DirectX::XMLoadFloat4(&offsets[i]);
	}
	for (int i = 0; i < 4; i++)
	{
		computeSSAOcbuff.frustumFarCorners[i] = frustumFarCorner[i];
	}
	return computeSSAOcbuff;
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

// 	D3D11_MAPPED_SUBRESOURCE mappedData;
// 	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pSSAOConstBuffer, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
// 	SSAOConstBuffer* pBuffer = reinterpret_cast<SSAOConstBuffer*>(mappedData.pData);
// 	pBuffer->viewToTexSpace = DirectX::XMMatrixTranspose(viewToTextureSpace);
// 	gfx.pgfx_pDeviceContext->Unmap(pSSAOConstBuffer, 0u);
}
