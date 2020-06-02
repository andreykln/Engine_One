#include "BoxAndCylinder.h"

BoxAndCylinder::BoxAndCylinder(Graphics& gfx)
{
	DirectX::XMFLOAT4 col{ 0.8f, 0.3f, 0.6f, 1.0f };
	box.CreateBox(0.5f, 0.5f, 0.5f, meshBox);
	cylinder.CreateCylinder(0.3f, 0.5f, 0.5f, 10u, 10u, meshCylinder);

	std::vector<Vertex_BandC> verticesBox(meshBox.vertices.size() + meshCylinder.vertices.size());

	UINT i = 0;
	for (; i < meshBox.vertices.size(); i++)
	{
		DirectX::XMFLOAT3 p = meshBox.vertices[i].position;
		verticesBox[i].pos = p;
		verticesBox[i].color = col;
	}

	for (size_t j = 0; j < meshCylinder.vertices.size(); j++)
	{
		DirectX::XMFLOAT3 c = meshCylinder.vertices[j].position;
		verticesBox[i].pos = c;
		verticesBox[i].color = col;
		i++;
	}

	VertexBuffer* pVB = new VertexBuffer(gfx, verticesBox, L"CylinderAndBox.");
	AddBind(pVB);

	VertexShader* pVertexShader = new VertexShader(gfx, L"Shader\\Vertex\\CubeVS.cso");
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


	PixelShader* pPixelShader = new PixelShader(gfx, L"Shaders\\Pixel\\CubePS.cso");
	AddBind(pPixelShader);


	std::vector<UINT> indices;
	for (auto&x : meshBox.indices)
	{
		indices.push_back(x);
	}

	for (auto& x : meshCylinder.indices)
	{
		indices.push_back(x);
	}

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, indices, L"CylinderIndexBuffer");
	AddIndexBuffer(pIndexBuffer);

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(pTopology);

	VertexConstantBuffer<DirectX::XMMATRIX>* pVCB =
		new VertexConstantBuffer<DirectX::XMMATRIX>(gfx, GetTransform() * gfx.GetProjection(), 0u, 1u);
	pCopyVertexConstantBuffer = pVCB->GetVertexConstantBuffer(); //for updating every frame
	AddBind(pVCB);

}

DirectX::XMMATRIX BoxAndCylinder::GetTransform() const noexcept
{
	return m_Matrix * m_Centered;
}

void BoxAndCylinder::Update(float dt) noexcept
{
	alpha = dt;
}

void BoxAndCylinder::UpdateVertexConstantBuffer(Graphics& gfx)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVertexConstantBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));

	DirectX::XMMATRIX* mat = reinterpret_cast<DirectX::XMMATRIX*>(mappedData.pData);
	*mat = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
	gfx.pgfx_pDeviceContext->Unmap(pCopyVertexConstantBuffer, 0u);
}


UINT BoxAndCylinder::GetBoxNumIndices() noexcept
{
	return UINT(meshBox.indices.size());
}

UINT BoxAndCylinder::GetCylinderNumIndices() noexcept
{
	return UINT(meshCylinder.indices.size());
}

UINT BoxAndCylinder::GetBoxNumVertices() noexcept
{
	return UINT(meshBox.vertices.size());
}
