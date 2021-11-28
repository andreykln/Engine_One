#include "Sky.h"

Sky::Sky(Graphics& gfx)
{
	geoGen.CreateSphere(5000.0f, 30, 30, mesh);

	std::vector<DirectX::XMFLOAT3> vertices(mesh.vertices.size());

	for (size_t i = 0; i < mesh.vertices.size(); i++)
	{
		vertices[i] = mesh.vertices[i].position;
	}

	VertexBuffer* pVB = new VertexBuffer(gfx, vertices, L"SkyVerticess");
	AddBind(pVB);

	IndexBuffer* pIB = new IndexBuffer(gfx, mesh.indices, L"SkyIndices_");
	AddIndexBuffer(pIB);

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(pTopology);


	std::wstring directory[1];
	directory[0] = L"Textures\\desertcube1024.dds";
	pCubeMapSRV = ShaderResourceView::CreateCubeMap(gfx, directory);


	VertexConstantBuffer<CB_VS_WorldViewProjection>* pVCBPerObject =
		new VertexConstantBuffer<CB_VS_WorldViewProjection>(gfx, WVPmatrix, 0u, 1u);
	pCopyWVPBuffer = pVCBPerObject->GetVertexConstantBuffer();
	AddBind(pVCBPerObject);


	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;

	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	DX::ThrowIfFailed(gfx.pgfx_pDevice->CreateSamplerState(&samplerDesc, &pSamplerState));


}

void Sky::UpdateVSMatricesAndCubeMap(Graphics& gfx, const DirectX::XMMATRIX& worldViewProj)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyWVPBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CB_VS_WorldViewProjection* pMatrices = reinterpret_cast<CB_VS_WorldViewProjection*>(mappedData.pData);
	pMatrices->worldViewProjection = DirectX::XMMatrixTranspose(worldViewProj);
	gfx.pgfx_pDeviceContext->Unmap(pCopyWVPBuffer, 0u);

	gfx.pgfx_pDeviceContext->PSSetShaderResources(0u, 1u, &pCubeMapSRV);
}

void Sky::DrawSky(Graphics& gfx, const DirectX::XMMATRIX& worldViewProj)
{
	UpdateVSMatricesAndCubeMap(gfx, worldViewProj);
	BindAndDrawIndexed(gfx);
}

ID3D11ShaderResourceView** Sky::GetSkyCubeMap()
{
	return &pCubeMapSRV;
}
