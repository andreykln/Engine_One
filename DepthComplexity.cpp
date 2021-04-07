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
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, indices, L"DepthIndexBuffer");
	AddIndexBuffer(pIndexBuffer);
	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(pTopology);

	VertexConstantBuffer<CB_VS_Transform>* pVCBPerObject =
		new VertexConstantBuffer<CB_VS_Transform>(gfx, transformMatrices, 0u, 1u);
	pCopyVCBMatricesDepth = pVCBPerObject->GetVertexConstantBuffer();
	AddBind(pVCBPerObject);

	PixelShaderConstantBuffer<CBPSDepth>* pPSCBDepth =
		new PixelShaderConstantBuffer<CBPSDepth>(gfx, depthColor, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyPSCBDepthColor = pPSCBDepth->GetPixelShaderConstantBuffer();
	AddBind(pPSCBDepth);
}

void DepthComplexity::UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBMatricesDepth, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CB_VS_Transform* pMatrices = reinterpret_cast<CB_VS_Transform*>(mappedData.pData);
	pMatrices->world = in_world;
	pMatrices->worldInvTranspose = MathHelper::InverseTranspose(in_world);
	pMatrices->worldViewProjection = DirectX::XMMatrixTranspose(in_world * in_ViewProj);
	pMatrices->texTransform = DirectX::XMMatrixIdentity();
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
