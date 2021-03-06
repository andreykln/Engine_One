#include "DepthComplexity.h"

DepthComplexity::DepthComplexity(Graphics& gfx)
{
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 col;
	};

	std::vector<Vertex> vertices(4);
	vertices[0].col = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
	vertices[1].col = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
	vertices[2].col = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
 	vertices[3].col = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);

	vertices[0].pos = DirectX::XMFLOAT3(30.0f, 30.0f, -5.0f);
	vertices[1].pos = DirectX::XMFLOAT3(30.0f, -30.0f, -5.0f);
	vertices[2].pos = DirectX::XMFLOAT3(-30.0f, -30.0f, -5.0f);
 	vertices[3].pos = DirectX::XMFLOAT3(-30.0f, 30.0f, -5.0f);


	VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, vertices, L"DepthPlane");
	AddBind(pVertexBuffer);

	std::vector<UINT> indices(6);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 3;
	indices[5] = 0;

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, indices, L"DepthIndexBuffer");
	AddIndexBuffer(pIndexBuffer);
	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(pTopology);

	VertexConstantBuffer<CBPerObject>* pVCBPerObject =
		new VertexConstantBuffer<CBPerObject>(gfx, matrices, 0u, 1u);
	pCopyVCBMatricesDepth = pVCBPerObject->GetVertexConstantBuffer(); //for updating every frame
	AddBind(pVCBPerObject);

	PixelShaderConstantBuffer<CBPSDepth>* pPSCBDepth =
		new PixelShaderConstantBuffer<CBPSDepth>(gfx, depthColor, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyPSCBDepthColor = pPSCBDepth->GetPixelShaderConstantBuffer();
	AddBind(pPSCBDepth);
}

DirectX::XMMATRIX DepthComplexity::GetTransform() const noexcept
{
	return m_Matrix * m_Centered;
}

void DepthComplexity::Update(float dt) noexcept
{
	alpha = dt;
}

void DepthComplexity::UpdateVertexConstantBuffer(Graphics& gfx)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBMatricesDepth, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CBPerObject* object = reinterpret_cast<CBPerObject*>(mappedData.pData);
	object->gWorld = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
	object->gWorldInvTranspose = MathHelper::InverseTranspose(object->gWorld);
	object->gWorldViewProj = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
	gfx.pgfx_pDeviceContext->Unmap(pCopyVCBMatricesDepth, 0u);
}

void DepthComplexity::UpdateDepthComplexityColor(Graphics& gfx, DirectX::XMFLOAT3 color)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;

	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPSCBDepthColor, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CBPSDepth* depth = reinterpret_cast<CBPSDepth*>(mappedData.pData);
	depth->depthColor.x = color.x;
	depth->depthColor.y = color.y;
	depth->depthColor.z = color.z;
	gfx.pgfx_pDeviceContext->Unmap(pCopyPSCBDepthColor, 0u);
}
