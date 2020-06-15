#include "HillsAndWaves.h"

HillsAndWaves::HillsAndWaves(Graphics& gfx)
{
	wave.Init(200, 200, 0.8f, 0.03f, 3.25f, 0.4f);
	generator.CreateGrid(160.0f, 160.0f, 50u, 50u, meshHills);

	verticesWaves.resize(wave.GetVertexCount());

	pDynamicVB = new VertexBufferDynamic(gfx, verticesWaves, L"Waves");
	pCopyDynamicVB = pDynamicVB->Get_p_DynamicVertexBuffer();
	AddBind(pDynamicVB);

	verticesHills.resize(meshHills.vertices.size());

	for (size_t i = 0; i < meshHills.vertices.size(); ++i)
	{
		DirectX::XMFLOAT3 p = meshHills.vertices[i].position;
		p.y = GetHeight(p.x, p.z);
		verticesHills[i].pos = p;
		//color is based on height
		if (p.y < -10.0f)
		{
			verticesHills[i].color = DirectX::XMFLOAT4(0.2f, 0.35f, 0.92f, 1.0f);
		}
		else if (p.y < 5.0f)
		{
			verticesHills[i].color = DirectX::XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
		}
		else if (p.y < 12.0f)
		{
			verticesHills[i].color = DirectX::XMFLOAT4(0.1f, 0.48f, 0.19f, 1.0f);
		}
		else if (p.y < 20.0f)
		{
			verticesHills[i].color = DirectX::XMFLOAT4(0.45f, 0.39f, 0.34f, 1.0f);
		}
		else
		{
			verticesHills[i].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}
	VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, verticesHills, L"Hills");
	AddBind(pVertexBuffer);

	VertexShader* pVertexShader = new VertexShader(gfx, L"CubeVS.cso");
	ID3DBlob* pVertexShaderBlob = pVertexShader->GetByteCode();
	AddBind(pVertexShader);

	const std::vector<D3D11_INPUT_ELEMENT_DESC> inputElemDesc =
	{
		{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"Color", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u}
	};

	InputLayout* pInputLayout = new InputLayout(gfx, pVertexShaderBlob, inputElemDesc, L"PositionAndColor");
	AddBind(pInputLayout);


	PixelShader* pPixelShader = new PixelShader(gfx, L"CubePS.cso");
	AddBind(pPixelShader);

	std::vector<UINT> indices(meshHills.indices.size() + 3 * (long long)wave.GetTriangleCount()); // 3 indices per face

	int k = 0;
	for (auto& x : meshHills.indices)
	{
		indices[k] = x;
		k++;
	}

// Iterate over each quad.
	UINT m = wave.GetRowCount();
	UINT n = wave.GetColumnCount();
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
	wavesIndicesNum = (UINT)indices.size();
	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, indices, L"WavesAndHillsIndexBuffer");
	AddIndexBuffer(pIndexBuffer);

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(pTopology);


	VertexConstantBuffer<DirectX::XMMATRIX>* pVCB =
		new VertexConstantBuffer<DirectX::XMMATRIX>(gfx, GetTransform() * gfx.GetProjection(), 0u, 1u);
	pCopyVertexConstantBuffer = pVCB->GetVertexConstantBuffer(); //for updating every frame
	AddBind(pVCB);
}

DirectX::XMMATRIX HillsAndWaves::GetTransform() const noexcept
{
	return m_Matrix * m_Centered;
}

void HillsAndWaves::Update(float dt) noexcept
{
	alpha = dt;
}

void HillsAndWaves::UpdateVertexConstantBuffer(Graphics& gfx)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVertexConstantBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));

	DirectX::XMMATRIX* mat = reinterpret_cast<DirectX::XMMATRIX*>(mappedData.pData);
	*mat = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
	gfx.pgfx_pDeviceContext->Unmap(pCopyVertexConstantBuffer, 0u);
}

void HillsAndWaves::UpdateScene(float totalTime, float dt, Graphics& gfx)
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
	Vertex_HAW* v = reinterpret_cast<Vertex_HAW*>(mappedData.pData);
	for (UINT i = 0; i < verticesWaves.size(); ++i)
	{
		v[i].pos = wave[i];
		v[i].color = DirectX::XMFLOAT4(0.0f, 0.0f, 0.4f, 1.0f);
	}
	gfx.pgfx_pDeviceContext->Unmap(pCopyDynamicVB, 0u);
}

UINT HillsAndWaves::GetHillsIndicesNum() noexcept
{
	return static_cast<UINT>(meshHills.indices.size());
}

UINT HillsAndWaves::GetWavesIndicesNum() noexcept
{
	auto a = meshWaves.indices.size();
	return static_cast<UINT>(meshWaves.indices.size());
}

UINT HillsAndWaves::GetHillsVerticesNum() noexcept
{
	return static_cast<UINT>(meshHills.vertices.size());
}

float HillsAndWaves::GetHeight(float x, float z) const
{
	return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z));
}
