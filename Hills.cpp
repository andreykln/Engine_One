#include "Hills.h"
#include <cmath>

Hills::Hills(Graphics& gfx, float in_width, float in_depth, UINT in_m, UINT in_n)
	: width(in_width), depth(in_depth), m(in_m), n(in_n)
{
	landscapeGenerated.CreateGrid(width, depth, m, n, grid);
	std::vector<Vertices_Full> vertices(grid.vertices.size());

	for (size_t i = 0; i < grid.vertices.size(); ++i)
	{
		DirectX::XMFLOAT3 p = grid.vertices[i].position;
		vertices[i].pos = p;
		vertices[i].normal = DirectX::XMFLOAT3{ 0.0f, 1.0f, 0.0f };
		vertices[i].tex = grid.vertices[i].TexC;
		vertices[i].tangent = grid.vertices[i].tangentU;
	}
	VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, vertices, L"Hills");
	AddBind(pVertexBuffer);

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, grid.indices, L"HillsIndexBuffer");
	AddIndexBuffer(pIndexBuffer);


	VertexConstantBuffer<ShadowMapGenVS>* pVCBSMGen =
		new VertexConstantBuffer<ShadowMapGenVS>(gfx, shadowMapCbuffer, 0u, 1u);
	pShadomMapGenCB = pVCBSMGen->GetVertexConstantBuffer();

	VertexConstantBuffer<cbDefaultVS>* pVCBPerObject =
		new VertexConstantBuffer<cbDefaultVS>(gfx, planeVSCB, 0u, 1u);
	pShadowMapVSDraw = pVCBPerObject->GetVertexConstantBuffer();

	planePSCB.mat.diffuseAlbedo = DirectX::XMFLOAT4(0.9f, 0.9, 0.9f, 1.0f);
	planePSCB.mat.fresnelR0 = DirectX::XMFLOAT3(0.2f, 0.2f, 0.2f);
	planePSCB.mat.shininess = 0.9f;
	planePSCB.dirLight.strength = DirectX::XMFLOAT3(0.8f, 0.8f, 0.8f);


	PixelShaderConstantBuffer<cbDefaultPS>* pLightsCB =
		new PixelShaderConstantBuffer<cbDefaultPS>(gfx, planePSCB, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pPlaneDrawPS = pLightsCB->GetPixelShaderConstantBuffer();


	std::wstring directory[1];
	std::wstring normalMap[1];

	directory[0] = L"Textures\\floor.dds";
	normalMap[0] = L"Textures\\floor_nmap.dds";

	ShaderResourceView* pSRVHeightMap = new ShaderResourceView(
		gfx, normalMap, 1u, (UINT)std::size(normalMap), ShaderType::Pixel);
	AddBind(pSRVHeightMap);

	ShaderResourceView* pSRV = new ShaderResourceView(gfx, directory, 0u, (UINT)std::size(directory), ShaderType::Pixel);
	AddBind(pSRV);

	TextureSampler* pTexSampler = new TextureSampler(gfx, ShaderType::Pixel);
	AddBind(pTexSampler);
}

void Hills::UpdateShadomMapGenBuffers(Graphics& gfx, const DirectX::XMMATRIX& in_lightWorld, DirectX::XMFLOAT3 newCamPosition)
{
	gfx.pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &pShadomMapGenCB);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pShadomMapGenCB, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	ShadowMapGenVS* pMatrices = reinterpret_cast<ShadowMapGenVS*>(mappedData.pData);
	pMatrices->lightWVP = DirectX::XMMatrixTranspose(in_lightWorld);
	pMatrices->texTransform = DirectX::XMMatrixIdentity();
	gfx.pgfx_pDeviceContext->Unmap(pShadomMapGenCB, 0u);
}

void Hills::UpdateShadowMapDrawBuffers(Graphics& gfx, DirectX::XMFLOAT3 newCamPosition,
	const DirectX::XMMATRIX& newShadowTransform, const DirectX::XMMATRIX& in_world,
	const DirectX::XMMATRIX& in_ViewProj, ID3D11ShaderResourceView* pShadowMapSRV, DirectX::XMFLOAT3& newLightDirection)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	gfx.pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &pShadowMapVSDraw);
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pShadowMapVSDraw, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	cbDefaultVS* shadowVS = reinterpret_cast<cbDefaultVS*> (mappedData.pData);
	shadowVS->texTransform = plateScaling;
	shadowVS->shadowTransform = newShadowTransform;
	shadowVS->world = DirectX::XMMatrixTranspose(in_world);
	shadowVS->worldInvTranspose = MathHelper::InverseTranspose(in_world);
	shadowVS->viewProjection = DirectX::XMMatrixTranspose(in_ViewProj);
	shadowVS->matTransform = DirectX::XMMatrixIdentity();
	shadowVS->enableDisplacementMapping = false;
	gfx.pgfx_pDeviceContext->Unmap(pShadowMapVSDraw, 0u);


	gfx.pgfx_pDeviceContext->PSSetConstantBuffers(0u, 1u, &pPlaneDrawPS);
	gfx.pgfx_pDeviceContext->PSSetShaderResources(2u, 1u, &pShadowMapSRV);
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pPlaneDrawPS, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));

	cbDefaultPS* surface = reinterpret_cast<cbDefaultPS*> (mappedData.pData);

	surface->camPositon = newCamPosition;
	surface->lightDirection = newLightDirection;
	gfx.pgfx_pDeviceContext->Unmap(pPlaneDrawPS, 0u);

}

