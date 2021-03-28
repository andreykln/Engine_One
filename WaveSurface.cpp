#include "WaveSurface.h"

WaveSurface::WaveSurface(Graphics& gfx)
{
	wave.Init(200, 200, 0.8f, 0.03f, 3.25f, 0.4f);

	perFrameLight.objectMaterial.ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	perFrameLight.objectMaterial.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);
	perFrameLight.objectMaterial.specular = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 32.0f);

	perFrameLight.dirLight[0].ambient = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 0.5f);
	perFrameLight.dirLight[0].diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	perFrameLight.dirLight[0].specular = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 0.5f);
	perFrameLight.dirLight[0].direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	perFrameLight.dirLight[1].ambient = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f);
	perFrameLight.dirLight[1].diffuse = DirectX::XMFLOAT4(0.20f, 0.20f, 0.20f, 0.5f);
	perFrameLight.dirLight[1].specular = DirectX::XMFLOAT4(0.65f, 0.65f, 0.65f, 0.5f);
	perFrameLight.dirLight[1].direction = DirectX::XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

	perFrameLight.dirLight[2].ambient = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f);
	perFrameLight.dirLight[2].diffuse = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 0.5f);
	perFrameLight.dirLight[2].specular = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 0.5f);
	perFrameLight.dirLight[2].direction = DirectX::XMFLOAT3(0.0f, -0.707f, -0.707f);

	vertices.resize(wave.GetVertexCount());

	pDynamicVB = new VertexBufferDynamic(gfx, vertices, L"Waves");
	pCopyDynamicVB = pDynamicVB->Get_p_DynamicVertexBuffer();
	AddBind(pDynamicVB);

	std::vector<UINT> indices(3 * (long long)wave.GetTriangleCount()); // 3 indices per face
// Iterate over each quad.
	UINT m = wave.GetRowCount();
	UINT n = wave.GetColumnCount();
	int k = 0;
	for (UINT i = 0; i < m - 1; ++i)
	{
		for (DWORD j = 0; j < n - 1; ++j)
		{
			indices[k] = i * n + j;
			indices[(long long)k + 1] = i * n + j + 1;
			indices[(long long)k + 2] = (i + 1) * n + j;
					
			indices[(long long)k + 3] = (i + 1) * n + j;
			indices[(long long)k + 4] = i * n + j + 1;
			indices[(long long)k + 5] = (i + 1) * n + j + 1;

			k += 6; // next quad
		}
	}


	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, indices, L"WaveSurfaceIndexBuffer");
	AddIndexBuffer(pIndexBuffer);

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(pTopology);


	VertexConstantBuffer<CB_VS_Transform>* pVCB =
		new VertexConstantBuffer<CB_VS_Transform>(gfx, transformMatrices, 0u, 1u);
	pCopyVertexConstantBuffer = pVCB->GetVertexConstantBuffer(); 
	AddBind(pVCB);

	PixelShaderConstantBuffer<CBPerFrame>* pPSCB =
		new PixelShaderConstantBuffer<CBPerFrame>(gfx, perFrameLight, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyPixelConstantBuffer = pPSCB->GetPixelShaderConstantBuffer();
	AddBind(pPSCB);

	PixelShaderConstantBuffer<CBFog>* pFog =
		new PixelShaderConstantBuffer<CBFog>(gfx, fogObj, 1u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	AddBind(pFog);


	std::wstring directory[1];
	directory[0] = L"Textures\\water2.dds";
	ShaderResourceView* pSRV = new ShaderResourceView(gfx, directory, (UINT)std::size(directory));
	AddBind(pSRV);

	TextureSampler* pTexSampler = new TextureSampler(gfx);
	AddBind(pTexSampler);

}

DirectX::XMMATRIX WaveSurface::GetTransform() const noexcept
{
	return m_Matrix * m_Centered;
}

void WaveSurface::Update(float dt) noexcept
{
	alpha = dt;
}

void WaveSurface::UpdateScene(float totalTime, float dt, Graphics& gfx)
{
	alpha = dt;
	// every quarter second, generate a random wave
	static float t_base{};
	if ((totalTime - t_base) >= 0.25f)
	{
		t_base += 0.25;
		unsigned long i = 5 + MathHelper::RandomIntWithingRange(0, INT_MAX) % (wave.GetRowCount() - 10);
		unsigned long j = 5 + MathHelper::RandomIntWithingRange(0, INT_MAX) % (wave.GetColumnCount() - 10);
		float magnitute = MathHelper::RandomFloatWithinRange(1.0f, 2.0f);
		wave.Disturb(i, j, magnitute);
	}
	wave.UpdateSolution(dt);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyDynamicVB, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	Vertex_IA* v = reinterpret_cast<Vertex_IA*>(mappedData.pData);
	for (UINT i = 0; i < vertices.size(); ++i)
	{
		v[i].pos = wave[i];
		v[i].normal = wave.Normal(i);
		//derive tex-coordinates in [0,1] from position
		v[i].tex.x = 0.5f + wave[i].x / wave.Width();
		v[i].tex.y = 0.5f - wave[i].z / wave.Depth();
	}
	gfx.pgfx_pDeviceContext->Unmap(pCopyDynamicVB, 0u);
}

void WaveSurface::UpdateVertexConstantBuffer(Graphics& gfx)
{
	waterTextureOffset.y += 0.05f * alpha;
	waterTextureOffset.x += 0.1f * alpha;
	wavesOffset = DirectX::XMMatrixTranslation(waterTextureOffset.x, waterTextureOffset.y, 0.0f);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVertexConstantBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CBPerObjectTexture* object = reinterpret_cast<CBPerObjectTexture*>(mappedData.pData);
	object->gWorld = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
	object->gWorldInvTranspose = MathHelper::InverseTranspose(object->gWorld);
	object->gWorldViewProj = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
	object->gTexTransform = wavesScale * wavesOffset;
	gfx.pgfx_pDeviceContext->Unmap(pCopyVertexConstantBuffer, 0u);

	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPixelConstantBuffer, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CBPerFrame* frame = reinterpret_cast<CBPerFrame*> (mappedData.pData);
	frame->cbEyePosition = eyePosition;
	if (GetAsyncKeyState('0') & 0x8000)
		frame->numLights = 0;
	if (GetAsyncKeyState('1') & 0x8000)
		frame->numLights = 1;

	if (GetAsyncKeyState('2') & 0x8000)
		frame->numLights = 2;

	if (GetAsyncKeyState('3') & 0x8000)
		frame->numLights = 3;
	gfx.pgfx_pDeviceContext->Unmap(pCopyPixelConstantBuffer, 0u);

}

void WaveSurface::UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj)
{
	waterTextureOffset.y += 0.05f * alpha;
	waterTextureOffset.x += 0.1f * alpha;
	wavesOffset = DirectX::XMMatrixTranslation(waterTextureOffset.x, waterTextureOffset.y, 0.0f);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVertexConstantBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CB_VS_Transform* object = reinterpret_cast<CB_VS_Transform*>(mappedData.pData);
	object->world = in_world;
	object->worldInvTranspose = MathHelper::InverseTranspose(in_world);
	object->worldViewProjection = DirectX::XMMatrixTranspose(in_world * in_ViewProj);
	object->texTransform = wavesScale * wavesOffset;
	gfx.pgfx_pDeviceContext->Unmap(pCopyVertexConstantBuffer, 0u);

}
