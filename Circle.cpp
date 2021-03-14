#include "circle.h"

Circle::Circle(Graphics& gfx)
{
	struct Lines
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 col;
	};

	std::vector<Lines> circleVertices;
	circleVertices.resize(3);
	circleVertices[0].col = DirectX::XMFLOAT4{ 0.5f, 0.5f, 0.5f, 1.0f };
	circleVertices[0].pos = DirectX::XMFLOAT3{ 0.5f, 0.5f, 0.5f};

	circleVertices[1].col = DirectX::XMFLOAT4{ 0.5f, 0.5f, 0.5f, 1.0f };
	circleVertices[1].pos = DirectX::XMFLOAT3{ 1.5f, 1.5f, 0.5f };

	circleVertices[2].col = DirectX::XMFLOAT4{ 0.5f, 0.5f, 0.5f, 1.0f };
	circleVertices[2].pos = DirectX::XMFLOAT3{ -0.5f, -0.5f, 0.5f };

	VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, circleVertices, L"CirclePositoins_");
	AddBind(pVertexBuffer);

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	AddBind(pTopology);
	VertexConstantBuffer<CBPerObject>* pCBMatrices = new VertexConstantBuffer<CBPerObject>(gfx, CBObject, 0u, 1u);
	pCopyVCBMatricesCircle = pCBMatrices->GetVertexConstantBuffer();
	AddBind(pCBMatrices);


}

DirectX::XMMATRIX Circle::GetTransform() const noexcept
{
	return DirectX::XMMatrixIdentity();
}

void Circle::Update(float dt) noexcept
{

}

void Circle::UpdateVertexConstantBuffer(Graphics& gfx)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBMatricesCircle, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CBPerObject* object = reinterpret_cast<CBPerObject*>(mappedData.pData);
	object->gWorld = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
	object->gWorldInvTranspose = MathHelper::InverseTranspose(object->gWorld);
	object->gWorldViewProj = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
	gfx.pgfx_pDeviceContext->Unmap(pCopyVCBMatricesCircle, 0u);
}
