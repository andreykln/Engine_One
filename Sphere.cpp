#include "Sphere.h"

Sphere::Sphere(Graphics& gfx, float radius, UINT sliceCount, UINT stackCount)
{
	struct Vertex_S
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
	};
	sphere.CreateSphere(radius, sliceCount, stackCount, mesh);
	std::vector<Vertex_S> vertices(mesh.vertices.size());
	DirectX::XMFLOAT4 col{ 0.5f, 0.6f, 0.2f, 1.0f };
	for (UINT i = 0; i < mesh.vertices.size(); i++)
	{
		DirectX::XMFLOAT3 p = mesh.vertices[i].position;
		vertices[i].pos = p;
		vertices[i].color = col;
	}

	VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, vertices, L"Sphere");
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

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, mesh.indices, L"SphereIndexBuffer");
	AddIndexBuffer(pIndexBuffer);

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(pTopology);

	VertexConstantBuffer<DirectX::XMMATRIX>* pVCB =
		new VertexConstantBuffer<DirectX::XMMATRIX>(gfx, GetTransform() * gfx.GetProjection(), 0u, 1u);
	pCopyVertexConstantBuffer = pVCB->GetVertexConstantBuffer(); //for updating every frame
	AddBind(pVCB);
}

DirectX::XMMATRIX Sphere::GetTransform() const noexcept
{
	return m_Matrix * m_Centered;
}

void Sphere::Update(float dt) noexcept
{
	alpha = dt;
}

void Sphere::UpdateVertexConstantBuffer(Graphics& gfx)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVertexConstantBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));

	DirectX::XMMATRIX* mat = reinterpret_cast<DirectX::XMMATRIX*>(mappedData.pData);
	*mat = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
	gfx.pgfx_pDeviceContext->Unmap(pCopyVertexConstantBuffer, 0u);
}
