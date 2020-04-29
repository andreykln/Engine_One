#include "WaveSurface.h"

WaveSurface::WaveSurface(Graphics& gfx)
{
	
	wave.Init(200, 200, 0.8f, 0.03f, 3.25f, 0.4f);

	vertices.resize(wave.GetVertexCount());
	pDynamicVB = std::make_unique<VertexBufferDynamic>(gfx, vertices, L"Waves");
	pCopyDynamicVB = pDynamicVB->Get_p_DynamicVertexBuffer();
	AddBind(std::move(pDynamicVB));


	auto pVertexShader = std::make_unique<VertexShader>(gfx, L"CubeVS.cso");
	auto pVertexShaderBlob = pVertexShader->GetByteCode();
	AddBind(std::move(pVertexShader));
	const std::vector<D3D11_INPUT_ELEMENT_DESC> inputElemDesc =
	{
		{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"Color", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u}
	};
	AddBind(std::make_unique<InputLayout>(gfx, pVertexShaderBlob, inputElemDesc, L"PositionAndColor"));
	AddBind(std::make_unique<PixelShader>(gfx, L"CubePS.cso"));

	std::vector<UINT> indices(3 * wave.GetTriangleCount()); // 3 indices per face

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

	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices, L"GeoSphereIndexBuffer"));
	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	AddBind(std::make_unique<TransformConstantBuffer>(gfx, *this));


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
	// every quarter second, generate a random wave
	static float t_base{};
	if ((totalTime - t_base) >= 0.25f)
	{
		t_base += 0.25;
		unsigned long i = 5 + MathHelper::RandomIntWithingRange(0, INT_MAX) % 190;
		unsigned long j = 5 + MathHelper::RandomIntWithingRange(0, INT_MAX) % 190;
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
		v[i].Color = DirectX::XMFLOAT4(0.0f, 0.0f, 0.4f, 1.0f);
	}
	gfx.pgfx_pDeviceContext->Unmap(pCopyDynamicVB, 0u);
}
