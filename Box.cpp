#include "Box.h"
#include "MathHelper.h"

#define RANDFLOAT float(rand()) / 1.0f

Box::Box(Graphics& gfx, float width, float height, float depth)
{
	box.CreateBox(width, height, depth, mesh);
	std::vector<Vertex_B> vertices(mesh.vertices.size());
	DirectX::XMFLOAT4 col{ 0.2f, 0.3f, 0.6f, 1.0f };
	vertices[0].color = col;
	for (UINT i = 0; i < mesh.vertices.size(); i++)
	{
		DirectX::XMFLOAT3 p = mesh.vertices[i].position;
		vertices[i].pos = p;
// 		vertices[i].color = col;
	}
	ColorBoxWithRainbow(vertices);
	VertexBuffer* pVB = new VertexBuffer(gfx, vertices, L"Box");
	AddBind(pVB);

	VertexShader* pVertexShader = new VertexShader(gfx, L"CubeVS.cso");
	ID3DBlob* pVertexShaderBlob = pVertexShader->GetByteCode();
	AddBind(pVertexShader); 

	
	const UINT offset = sizeof(DirectX::XMFLOAT3) * 3 + sizeof(DirectX::XMFLOAT2) * 2;

	const std::vector<D3D11_INPUT_ELEMENT_DESC> inputElemDesc =
	{
		{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"Color", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, offset,
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

	RasterizerState state;
	Rasterizer* pRasterState = new Rasterizer(gfx, state.SolidFill());
	AddBind(pRasterState);

}

DirectX::XMMATRIX Box::GetTransform() const noexcept
{
	return m_Matrix * m_Centered;
}

void Box::Update(float dt) noexcept
{
	alpha = dt;
}

void Box::ColorBoxWithRainbow(std::vector<Vertex_B>& vertices) noexcept
{
	vertices[0].color = { 1.0f, 0.0f, 0.0f, 1.0f };
	vertices[1].color = { 0.0f, 1.0f, 0.0f, 1.0f };
	vertices[2].color = { 0.0f, 0.0f, 1.0f, 1.0f };

	vertices[3].color = { 1.0f, 0.0f, 0.0f, 1.0f };
	vertices[4].color = { 0.0f, 1.0f, 0.0f, 1.0f };
	vertices[5].color = { 0.0f, 0.0f, 1.0f, 1.0f };

	vertices[6].color = { 0.5f, 0.0f, 0.5f, 1.0f };
	vertices[7].color = { 0.0f, 0.5f, 0.3f, 1.0f };

}
