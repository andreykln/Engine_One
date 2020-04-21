#include "Sphere.h"

Sphere::Sphere(Graphics& gfx, float in_x, DirectX::XMMATRIX in_MatrixStartPosition)
	: alpha(in_x), m_Translation(in_MatrixStartPosition)
{
	struct Vertex_S
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
	};
	sphere.CreateSphere(0.75f, 20u, 10u, mesh);
	std::vector<Vertex_S> vertices(mesh.vertices.size());
	DirectX::XMFLOAT4 col{ 0.5f, 0.6f, 0.2f, 1.0f };
	for (UINT i = 0; i < mesh.vertices.size(); i++)
	{
		DirectX::XMFLOAT3 p = mesh.vertices[i].position;
		vertices[i].pos = p;
		vertices[i].color = col;
	}

	AddBind(std::make_unique<VertexBuffer>(gfx, vertices, L"Sphere"));
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
	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, mesh.indices, L"SphereIndexBuffer"));
	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	AddBind(std::make_unique<TransformConstantBuffer>(gfx, *this));
}

DirectX::XMMATRIX Sphere::GetTransform() const noexcept
{

	return Shape::m_Matrix * m_Translation *
		m_Centered;
// 	return DirectX::XMMatrixRotationX(alpha) * 
// 		DirectX::XMMatrixTranslation(0.0f, 0.0f, 4.0f);
}

// DirectX::XMMATRIX Sphere::Update(float dt, DirectX::XMMATRIX in_matrix) noexcept
// {
// 	alpha = dt;
// 	return in_matrix;
// }