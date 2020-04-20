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
	AddBind(std::make_unique<VertexBuffer>(gfx, vertices, L"Box"));
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
	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, mesh.indices, L"BoxIndexBuffer"));
	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_unique<TransformConstantBuffer>(gfx, *this));


}

DirectX::XMMATRIX Box::GetTransform() const noexcept
{
	return m_Matrix * 
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 4.0f);
}


DirectX::XMMATRIX Box::Update(float dt, DirectX::XMMATRIX in_matrix) noexcept
{
	alpha = dt;
	m_Matrix *= in_matrix;
	return m_Matrix;
}

float Box::GetAlpha()
{
	return alpha;
}
