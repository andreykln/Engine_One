#include "Cube.h"

Cube::Cube(Graphics& gfx, float& in_x, float& in_y, float& in_z)
	: x(in_x), y(in_y), z(in_z)
{



	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
	};
	const std::vector<Vertex> cubeCoord
	{
		{DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f),	DirectX::XMFLOAT4(DirectX::Colors::Red)},
		{DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f),		DirectX::XMFLOAT4(DirectX::Colors::AliceBlue)},
		{DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f),		DirectX::XMFLOAT4(DirectX::Colors::Beige)},

		{DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f),		DirectX::XMFLOAT4(DirectX::Colors::Cyan)},
		{DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f),		DirectX::XMFLOAT4(DirectX::Colors::Honeydew)},
		{DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f),		DirectX::XMFLOAT4(DirectX::Colors::LightGray)},

		{DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f),		DirectX::XMFLOAT4(DirectX::Colors::OliveDrab)},
		{DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f),		DirectX::XMFLOAT4(DirectX::Colors::WhiteSmoke)}
	};
	AddBind(std::make_unique<VertexBuffer>(gfx, cubeCoord, L"Cube coordinates"));
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

	const std::vector<UINT> indices
	{
		//front
		0,2,1, 0,3,2,
		//back
		7,5,6, 6,5,4,
		//right side
		1,2,7, 2,5,7,
		//left side
		6,3,0, 6,4,3,
		//up
		2,4,5, 3,4,2,
		//down
		1,7,6, 1,6,0
	};
	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices, L"CubeIndBuff"));
	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	AddBind(std::make_unique<TransformConstantBuffer>(gfx, *this));
}

DirectX::XMMATRIX Cube::GetTransform() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(alpha, 0.0f, alpha) * 
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 4.0f);
}

// DirectX::XMMATRIX Cube::Update(float dt, DirectX::XMMATRIX in_matrix) noexcept
// {
// 	alpha = dt;
// 	return in_matrix;
// }
