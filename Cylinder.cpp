#include "Cylinder.h"
Cylinder::Cylinder(Graphics& gfx, float in_x)
	: alpha(in_x)
{
	struct Vertex_C
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
	};
	cylinderParts.CreateCylinder(0.5f, 0.5f, 1.0f, 20u, 20u, mesh);
	std::vector<Vertex_C> vertices(mesh.vertices.size());
	DirectX::XMFLOAT4 col{ 0.5f, 0.8f, 0.2f, 1.0f };

	for (UINT i = 0; i < mesh.vertices.size(); i++)
	{
		DirectX::XMFLOAT3 p = mesh.vertices[i].position;
		vertices[i].pos = p;
		vertices[i].color = col;
	}

	AddBind(std::make_unique<VertexBuffer>(gfx, vertices, L"Cylinder"));

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

	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, mesh.indices, L"CylinderIndexBuffer"));

	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	AddBind(std::make_unique<TransformConstantBuffer>(gfx, *this));

}

DirectX::XMMATRIX Cylinder::GetTransform() const noexcept
{
	return DirectX::XMMatrixRotationX(alpha) * 
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 4.0f);
}

void Cylinder::Update(float dt) noexcept
{
	alpha = dt;
}
