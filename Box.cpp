#include "Box.h"
#include "MathHelper.h"
#define RANDFLOAT float(rand()) / 1.0f

Box::Box(Graphics& gfx, float width, float height, float depth)
{
	struct Vertex_B
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
	};
	box.CreateBox(width, height, depth, mesh);
	std::vector<Vertex_B> vertices(mesh.vertices.size());
	DirectX::XMFLOAT4 col{ 0.2f, 0.3f, 0.6f, 1.0f };
	vertices[0].color = col;
	for (UINT i = 0; i < mesh.vertices.size(); i++)
	{
		DirectX::XMFLOAT3 p = mesh.vertices[i].position;
		vertices[i].pos = p;
		vertices[i].color = col;
	}

	VertexBuffer* pVB = new VertexBuffer(gfx, vertices, L"Box");
	AddBind(pVB);

	VertexShader* pVertexShader = new VertexShader(gfx, L"CubeVS.cso");
	ID3DBlob* pVertexShaderBlob = pVertexShader->GetByteCode();
	AddBind(pVertexShader); //std::move(pVertexShader

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

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, mesh.indices, L"BoxIndexBuffer");
	AddIndexBuffer(pIndexBuffer);

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(pTopology);

	TransformConstantBuffer* pTransformConstBuff = new TransformConstantBuffer(gfx, *this);
	AddBind(pTransformConstBuff);
}

DirectX::XMMATRIX Box::GetTransform() const noexcept
{
	return m_Matrix * m_Centered;
}

void Box::Update(float dt) noexcept
{
	alpha = dt;
}
