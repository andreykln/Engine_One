#include "WaveSurface.h"

WaveSurface::WaveSurface(Graphics& gfx)
{
	wave.Init(200, 200, 0.8f, 0.03f, 3.25f, 0.4f);

	directionalLight.mat.ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	directionalLight.mat.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);
	directionalLight.mat.specular = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 32.0f);

	directionalLight.dirLight[0].ambient = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 0.5f);
	directionalLight.dirLight[0].diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	directionalLight.dirLight[0].specular = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 0.5f);
	directionalLight.dirLight[0].direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	directionalLight.dirLight[1].ambient = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f);
	directionalLight.dirLight[1].diffuse = DirectX::XMFLOAT4(0.20f, 0.20f, 0.20f, 0.5f);
	directionalLight.dirLight[1].specular = DirectX::XMFLOAT4(0.65f, 0.65f, 0.65f, 0.5f);
	directionalLight.dirLight[1].direction = DirectX::XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

	directionalLight.dirLight[2].ambient = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f);
	directionalLight.dirLight[2].diffuse = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 0.5f);
	directionalLight.dirLight[2].specular = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 0.5f);
	directionalLight.dirLight[2].direction = DirectX::XMFLOAT3(0.0f, -0.707f, -0.707f);

	directionalLight.mat.ambient = DirectX::XMFLOAT4(0.0f, 0.427f, 0.557f, 1.0f);
	directionalLight.mat.diffuse = DirectX::XMFLOAT4(0.0f, 0.427f, 0.557f, 1.0f);
	directionalLight.mat.specular = DirectX::XMFLOAT4(0.55f, 0.55f, 0.55f, 64.0f);
	directionalLight.mat.reflect = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

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

	PixelShaderConstantBuffer<CB_PS_DirectionalL_Fog>* pPSCB =
		new PixelShaderConstantBuffer<CB_PS_DirectionalL_Fog>(gfx, directionalLight, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	AddBind(pPSCB);

	;	PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>* pLightsCB =
		new PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>(gfx, pscBuffer, 1u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyPixelConstantBuffer = pLightsCB->GetPixelShaderConstantBuffer();
	AddBind(pLightsCB);



	std::wstring directory[1];
	directory[0] = L"Textures\\water2.dds";
	ShaderResourceView* pSRV = new ShaderResourceView(gfx, directory, (UINT)std::size(directory));
	AddBind(pSRV);

	TextureSampler* pTexSampler = new TextureSampler(gfx);
	AddBind(pTexSampler);

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

void WaveSurface::UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPositon)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPixelConstantBuffer, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CB_PS_PerFrameUpdate* frame = reinterpret_cast<CB_PS_PerFrameUpdate*> (mappedData.pData);
	frame->cameraPositon = camPositon;

	if (GetAsyncKeyState('0') & 0x8000)
		frame->numberOfLights = 0;
	if (GetAsyncKeyState('1') & 0x8000)
		frame->numberOfLights = 1;
	if (GetAsyncKeyState('2') & 0x8000)
		frame->numberOfLights = 2;
	if (GetAsyncKeyState('3') & 0x8000)
		frame->numberOfLights = 3;

	gfx.pgfx_pDeviceContext->Unmap(pCopyPixelConstantBuffer, 0u);
}

DirectX::XMMATRIX WaveSurface::GetWaveSurfaceOffset()
{
	return wavesSurfaceOffset;
}
