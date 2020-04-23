#include "LoadModelFromTXT.h"

LoadModelFromTXT::LoadModelFromTXT(Graphics& gfx, const std::wstring& path)
{
	struct Vertices
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};
	UINT vertices = 0;
	UINT triangles = 0;

	std::fstream file(path);
	std::string ignore;
	file >> ignore >> vertices;
	file >> ignore >> triangles;
	file >> ignore >> ignore >> ignore >> ignore;
	float nx, ny, nz; //for ignoring normals for now
	std::vector<Vertices> verticesFromTXT(vertices);
	DirectX::XMFLOAT4 black(0.0f, 0.0f, 0.0f, 1.0f);
	for (size_t i = 0; i < vertices; i++)
	{
		file >> verticesFromTXT[i].position.x >> verticesFromTXT[i].position.y >> verticesFromTXT[i].position.z;
		verticesFromTXT[i].color = black;
		file >> nx >> ny >> nz;
	}
	file >> ignore >> ignore >> ignore;
	UINT indexCount = 3 * triangles;
	std::vector<UINT> indices(indexCount);

	for (size_t i = 0; i < triangles; ++i)
	{
		file >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}
	file.close();

	AddBind(std::make_unique<VertexBuffer>(gfx, verticesFromTXT, L"TXTFile"));
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
	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices, L"TXTIndexBuffer"));
	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	AddBind(std::make_unique<TransformConstantBuffer>(gfx, *this));

}

DirectX::XMMATRIX LoadModelFromTXT::GetTransform() const noexcept
{
	return m_Matrix * m_Centered;
}

void LoadModelFromTXT::Update(float dt) noexcept
{
	alpha = dt;
}
