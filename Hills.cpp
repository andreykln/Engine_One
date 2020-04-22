#include "Hills.h"
#include <cmath>

Hills::Hills(Graphics& gfx, float in_width, float in_depth, UINT in_m, UINT in_n)
	: width(in_width), depth(in_depth), m(in_m), n(in_n)
{
	struct Vertex_l
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
	};


	landscapeGenerated.CreateGrid(width, depth, m, n, grid);
	std::vector<Vertex_l> vertices(grid.vertices.size());
	
	for (size_t i = 0; i < grid.vertices.size(); ++i) 
	{
		DirectX::XMFLOAT3 p = grid.vertices[i].position;
// 		p.y = GetHeight(p.x, p.z);
		vertices[i].pos = p;
		//color is based on height
		if (p.y < -10.0f)
		{
			vertices[i].color = DirectX::XMFLOAT4(0.2f, 0.35f, 0.92f, 1.0f);
		}
		else if (p.y < 5.0f)
		{
			vertices[i].color = DirectX::XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
		}
		else if (p.y < 12.0f)
		{
			vertices[i].color = DirectX::XMFLOAT4(0.1f, 0.48f, 0.19f, 1.0f);
		}
		else if (p.y < 20.0f)
		{
			vertices[i].color = DirectX::XMFLOAT4(0.45f, 0.39f, 0.34f, 1.0f);
		}
		else
		{
			vertices[i].color = DirectX::XMFLOAT4(1.f, 1.0f, 1.0f, 1.0f);
		}
	}

	AddBind(std::make_unique<VertexBuffer>(gfx, vertices, L"Hills landscape"));
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

	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, grid.indices, L"HillsIndBuff"));
	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	AddBind(std::make_unique<TransformConstantBuffer>(gfx, *this));

}


DirectX::XMMATRIX Hills::GetTransform() const noexcept
{
	return m_Matrix * m_Centered;;
}

void Hills::Update(float dt) noexcept
{
	alpha = dt;
}

void Hills::SetWidth(float in_width) noexcept
{
	width = in_width;
}

void Hills::SetDepth(float in_depth) noexcept
{
	depth = in_depth;
}

void Hills::SetVerticesWidth(UINT in_vertWidth) noexcept
{
	m = in_vertWidth;
}

void Hills::SetVerticesDepth(UINT in_vertDepth) noexcept
{
	n = in_vertDepth;
}

float Hills::GetHeight(float x, float z) const
{
	return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z));
}
