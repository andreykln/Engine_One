#include "Pyramid.h"

Pyramid::Pyramid(Graphics& gfx, float bottom_side, float height)
{
	struct Vertex_P
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};
	pyramid.CreatePyramid(bottom_side, height, mesh);
	std::vector<Vertex_P> vertices(mesh.vertices.size());
	DirectX::XMFLOAT4 red{ 0.9f, 0.1f, 0.1f, 1.0f };
	DirectX::XMFLOAT4 green{ 0.1f, 0.9f, 0.1f, 1.0f };
	for (UINT i = 0; i < mesh.vertices.size(); i++)
	{
		DirectX::XMFLOAT3 p = mesh.vertices[i].position;
		vertices[i].position = p;
		if(i < 4)
		vertices[i].color = green;
		else 
		vertices[i].color = red;
	}
	VertexBuffer* pVB = new VertexBuffer(gfx, vertices, L"Pyramid");
	AddBind(pVB);

	VertexShader* pVertexShader = new VertexShader(gfx, L"PyramidDistorted.cso");
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

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, mesh.indices, L"PyramidIndexBuffer");
	AddIndexBuffer(pIndexBuffer);

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(pTopology);

	TransformConstantBuffer* pTransformConstBuff = new TransformConstantBuffer(gfx, *this);
	AddBind(pTransformConstBuff);


	RasterizerState state;
	Rasterizer* pRasterState = new Rasterizer(gfx, state.Wireframe());
	AddBind(pRasterState);
}

DirectX::XMMATRIX Pyramid::GetTransform() const noexcept
{
	return m_Matrix * m_Centered;
}

void Pyramid::Update(float dt) noexcept
{
	alpha = dt;
}
