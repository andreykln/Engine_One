#include "circle.h"

Circle::Circle(Graphics& gfx)
{
	struct Lines
	{
		DirectX::XMFLOAT3 pos;
	};
	std::vector<Lines> circleVertices;
	geoGenerator.CreateBox(1.0f, 1.0f, 1.0f, mesh);

	circleVertices.resize(segments);
	float offset = 0.5f;
	circleVertices[0].pos = DirectX::XMFLOAT3{ offset, 0.0f, 0.0f };
	circleVertices[1].pos = DirectX::XMFLOAT3{ 0.0f, offset, 0.0f };
	circleVertices[2].pos = DirectX::XMFLOAT3{ -offset, 0.0f, 0.0f };
// 	circleVertices[3].pos = DirectX::XMFLOAT3{ -offset, 0.0f, 0.0f };

// 	float radius = 5;
// 	const float alpha = DirectX::XM_2PI / segments;
// 	float angle = alpha;
// 	UINT segment = 1;
// 	for (UINT i = 0; i < segments; i++)
// 	{
// 		circleVertices[i].pos.x = sin(angle) * radius;
// 		circleVertices[i].pos.y = 0.0f;
// 		circleVertices[i].pos.z = cos(angle) * radius;
// 		angle += alpha;
// 	}
// 	//circle won't connects, so connect last vertex with the first one manually
// 	//with big enough amount of vertices (200+) this is not noticeable
// 	circleVertices[segments - 1].pos = circleVertices[0].pos;

	VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, mesh.vertices, L"CirclePositoins_");
	AddBind(pVertexBuffer);

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(pTopology);
	VertexConstantBuffer<CBPerObject>* pCBMatrices = new VertexConstantBuffer<CBPerObject>(gfx, CBObject, 0u, 1u);
	pCopyVCBMatricesCircle = pCBMatrices->GetVertexConstantBuffer();
	AddBind(pCBMatrices);

	std::vector<UINT> indices(3);
	indices[0] = 2;
	indices[1] = 1;
	indices[2] = 0;


	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, mesh.indices, L"BoxIndexBuffer.");
	AddIndexBuffer(pIndexBuffer);


}

DirectX::XMMATRIX Circle::GetTransform() const noexcept
{
	//using scaling for circumvent shitty matrix usage, it should fix itself later
	return DirectX::XMMatrixScaling(0.75f, 1.0f, 1.0f);
}

void Circle::Update(float dt) noexcept
{

}

void Circle::UpdateVertexConstantBuffer(Graphics& gfx)
{
// 	D3D11_MAPPED_SUBRESOURCE mappedData;
// 	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBMatricesCircle, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
// 	CBPerObject* object = reinterpret_cast<CBPerObject*>(mappedData.pData);
// 	object->gWorld = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
// 	object->gWorld = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());

// 	object->gWorld = GetTransform() * gfx.GetProjection();

// 	object->gWorldInvTranspose = MathHelper::InverseTranspose(object->gWorld);
// 	object->gWorldViewProj = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
// 	gfx.pgfx_pDeviceContext->Unmap(pCopyVCBMatricesCircle, 0u);
}

void Circle::UpdateVSMatrices(Graphics& gfx, DirectX::XMMATRIX matrix)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBMatricesCircle, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CBPerObject* object = reinterpret_cast<CBPerObject*>(mappedData.pData);
// 	object->gWorld = DirectX::XMMatrixTranspose(matrix);
	object->gWorld = matrix;

	object->gWorldInvTranspose = MathHelper::InverseTranspose(object->gWorld);
	object->gWorldViewProj = DirectX::XMMatrixTranspose(matrix);
	gfx.pgfx_pDeviceContext->Unmap(pCopyVCBMatricesCircle, 0u);
}

UINT Circle::GetVertices() const
{
	return segments;
}
