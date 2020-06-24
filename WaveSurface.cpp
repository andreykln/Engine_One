#include "WaveSurface.h"

WaveSurface::WaveSurface(Graphics& gfx)
{
	wave.Init(200, 200, 0.8f, 0.03f, 3.25f, 0.4f);

	perFrameLight.objectMaterial.ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	perFrameLight.objectMaterial.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	perFrameLight.objectMaterial.specular = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 32.0f);

	perFrameLight.dirLight[0].ambient = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	perFrameLight.dirLight[0].diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	perFrameLight.dirLight[0].specular = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	perFrameLight.dirLight[0].direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	perFrameLight.dirLight[1].ambient = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	perFrameLight.dirLight[1].diffuse = DirectX::XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
	perFrameLight.dirLight[1].specular = DirectX::XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	perFrameLight.dirLight[1].direction = DirectX::XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

	perFrameLight.dirLight[2].ambient = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	perFrameLight.dirLight[2].diffuse = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	perFrameLight.dirLight[2].specular = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	perFrameLight.dirLight[2].direction = DirectX::XMFLOAT3(0.0f, -0.707f, -0.707f);



	vertices.resize(wave.GetVertexCount());




	pDynamicVB = new VertexBufferDynamic(gfx, vertices, L"Waves");
	pCopyDynamicVB = pDynamicVB->Get_p_DynamicVertexBuffer();
	AddBind(pDynamicVB);

	VertexShader* pVertexShader = new VertexShader(gfx, L"Shaders\\Vertex\\CubeLightTexVS.cso");
	ID3DBlob* pVertexShaderBlob = pVertexShader->GetByteCode();
	AddBind(pVertexShader);

	const std::vector<D3D11_INPUT_ELEMENT_DESC> inputElemDesc =
	{
		{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"TexCoordinate", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"Normal", 0u, DXGI_FORMAT_R8G8B8A8_UNORM, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u}
	};

	InputLayout* pInputLayout = new InputLayout(gfx, pVertexShaderBlob, inputElemDesc, L"PositionAndColor");
	AddBind(pInputLayout);


	PixelShader* pPixelShader = new PixelShader(gfx, L"Shaders\\Pixel\\CubeLightTexPS.cso");
	AddBind(pPixelShader);

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


	VertexConstantBuffer<CBPerObjectTexture>* pVCB =
		new VertexConstantBuffer<CBPerObjectTexture>(gfx, perObjectMatrices, 0u, 1u);
	pCopyVertexConstantBuffer = pVCB->GetVertexConstantBuffer(); //for updating every frame
	AddBind(pVCB);

	PixelShaderConstantBuffer<CBPerFrame>* pPSCB =
		new PixelShaderConstantBuffer<CBPerFrame>(gfx, perFrameLight, 0u, 1u);
	pCopyPixelConstantBuffer = pPSCB->GetPixelShaderConstantBuffer();
	AddBind(pPSCB);

	ShaderResourceView* pSRV = new ShaderResourceView(gfx, L"Textures\\water2.dds");
	AddBind(pSRV);

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
	Vertex* v = reinterpret_cast<Vertex*>(mappedData.pData);
	for (UINT i = 0; i < vertices.size(); ++i)
	{
		v[i].Pos = wave[i];
		v[i].Normal = wave.Normal(i);
		//derive tex-coordinates in [0,1] from position
		v[i].Tex.x = 0.5f + wave[i].x / wave.Width();
		v[i].Tex.y = 0.5f - wave[i].z / wave.Depth();
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
