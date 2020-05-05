#include "Cylinder.h"
Cylinder::Cylinder(Graphics& gfx,
	float bottom_radius, float top_radius, float height, UINT slice_count, UINT stack_count )
{
	struct Vertex_C
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
	};
	cylinderParts.CreateCylinder(bottom_radius, top_radius, height, slice_count, stack_count, mesh);
	std::vector<Vertex_C> vertices(mesh.vertices.size());
	DirectX::XMFLOAT4 col{ 1.0f, 1.0f, 1.0f, 1.0f };

	for (UINT i = 0; i < mesh.vertices.size(); i++)
	{
		DirectX::XMFLOAT3 p = mesh.vertices[i].position;
		vertices[i].pos = p;
		vertices[i].color = col;
	}

	VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, vertices, L"Cylinder");
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

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, mesh.indices, L"CylinderIndexBuffer");
	AddIndexBuffer(pIndexBuffer);

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(pTopology);

	TransformConstantBuffer* pTransformConstBuff = new TransformConstantBuffer(gfx, *this);
	AddBind(pTransformConstBuff);
}

DirectX::XMMATRIX Cylinder::GetTransform() const noexcept
{
	return m_Matrix * m_Centered;
}

void Cylinder::Update(float dt) noexcept
{
	alpha = dt;
}

