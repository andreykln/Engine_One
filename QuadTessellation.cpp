#include "QuadTessellation.h"

QuadTessellation::QuadTessellation(Graphics& gfx)
{

	std::vector<DirectX::XMFLOAT3> vertices
	{
		// Row 0
		DirectX::XMFLOAT3(-10.0f, -10.0f, +15.0f),
		DirectX::XMFLOAT3(-5.0f,  0.0f, +15.0f),
		DirectX::XMFLOAT3(+5.0f,  0.0f, +15.0f),
		DirectX::XMFLOAT3(+10.0f, 0.0f, +15.0f),

		// Row 1
		DirectX::XMFLOAT3(-15.0f, 0.0f, +5.0f),
		DirectX::XMFLOAT3(-5.0f,  0.0f, +5.0f),
		DirectX::XMFLOAT3(+5.0f,  20.0f, +5.0f),
		DirectX::XMFLOAT3(+15.0f, 0.0f, +5.0f),

		// Row 2
		DirectX::XMFLOAT3(-15.0f, 0.0f, -5.0f),
		DirectX::XMFLOAT3(-5.0f,  0.0f, -5.0f),
		DirectX::XMFLOAT3(+5.0f,  0.0f, -5.0f),
		DirectX::XMFLOAT3(+15.0f, 0.0f, -5.0f),

		// Row 3
		DirectX::XMFLOAT3(-10.0f, 10.0f, -15.0f),
		DirectX::XMFLOAT3(-5.0f,  0.0f, -15.0f),
		DirectX::XMFLOAT3(+5.0f,  0.0f, -15.0f),
		DirectX::XMFLOAT3(+25.0f, 10.0f, -15.0f)
	};
	VertexBuffer* pVB = new VertexBuffer(gfx, vertices, L"QuadTessellation.");
	AddBind(pVB);

	Topology* topology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST);
	AddBind(topology);


	HullShaderConstantBuffer<CB_QuadTess_HS>* pHSCB = 
		new HullShaderConstantBuffer(gfx, cbHullShader, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyCBHS = pHSCB->GetHullShaderConstantBuffer();
	AddBind(pHSCB);

	DomainShaderConstantBuffer<CB_QuadTess_DS_WVP>* pDSCH =
		new DomainShaderConstantBuffer(gfx, cbDSworldViewProj, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyCBDS = pDSCH->GetDomainShaderConstantBuffer();
	AddBind(pDSCH);

}

void QuadTessellation::UpdateTessellationShaderBuffers(Graphics& gfx,
	DirectX::XMMATRIX WVP, DirectX::XMMATRIX world, DirectX::XMFLOAT3 cameraPos)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyCBHS, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CB_QuadTess_HS* pMatrices = reinterpret_cast<CB_QuadTess_HS*>(mappedData.pData);
	pMatrices->world = world;
	pMatrices->cameraPosition = cameraPos;
	gfx.pgfx_pDeviceContext->Unmap(pCopyCBHS, 0u);


	D3D11_MAPPED_SUBRESOURCE mappedData0;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyCBDS, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData0));
	CB_QuadTess_DS_WVP* pWorld = reinterpret_cast<CB_QuadTess_DS_WVP*>(mappedData0.pData);
	pWorld->worldViewProjection = DirectX::XMMatrixTranspose(WVP);
	gfx.pgfx_pDeviceContext->Unmap(pCopyCBDS, 0u);

}
