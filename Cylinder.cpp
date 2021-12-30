#include "Cylinder.h"
Cylinder::Cylinder(Graphics& gfx,
	float bottom_radius, float top_radius, float height, UINT slice_count, UINT stack_count)
{
	cylinderParts.CreateCylinder(bottom_radius, top_radius, height, slice_count, stack_count, mesh);
	std::vector<Vertex_IA> vertices(mesh.vertices.size());


	std::vector<Vertices_Full> verticesWithNormals(mesh.vertices.size());

	for (UINT i = 0; i < mesh.vertices.size(); i++)
	{
		DirectX::XMFLOAT3 p = mesh.vertices[i].position;
		DirectX::XMFLOAT3 n = mesh.vertices[i].normal;
		DirectX::XMFLOAT2 t = mesh.vertices[i].TexC;
		DirectX::XMFLOAT3 tg = mesh.vertices[i].tangentU;

		verticesWithNormals[i].pos = p;
		verticesWithNormals[i].normal = n;
		verticesWithNormals[i].tex = t;
		verticesWithNormals[i].tangent = tg;
	}

	//build world matrices for 10 columns
	for (int i = 0; i < 5; ++i)
	{
		DirectX::XMStoreFloat4x4(&sCylWorld[i * 2 + 0],
			DirectX::XMMatrixTranslation(-5.0f, 1.5f, -10.0f + i * 5.0f));

		DirectX::XMStoreFloat4x4(&sCylWorld[i * 2 + 1],
			DirectX::XMMatrixTranslation(5.0f, 1.5f, -10.0f + i * 5.0f));
	}
	std::vector<DirectX::XMFLOAT4X4> cylWorlds(10);
	for (size_t i = 0; i < 10; i++)
	{
		cylWorlds[i] = sCylWorld[i];
	}

	VertexBufferInstancedDynamic* pVertexBuffer = new VertexBufferInstancedDynamic(gfx, verticesWithNormals, cylWorlds, L"IstancedVB");
	pIAbuffers[0] = pVertexBuffer->GetVertexData();
	pIAbuffers[1] = pVertexBuffer->GetInstancedData();
	stride[0] = sizeof(Vertices_Full);
	stride[1] = sizeof(DirectX::XMFLOAT4X4);


	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, mesh.indices, L"CylinderIndexBuffer");
	AddIndexBuffer(pIndexBuffer);


	VertexConstantBuffer<cbDefaultVS>* pVCBPerObject =
		new VertexConstantBuffer<cbDefaultVS>(gfx, coneVSCB, 0u, 1u);
	pShadowMapVSDraw = pVCBPerObject->GetVertexConstantBuffer();

	VertexConstantBuffer<cbCreateNormalMapInstanced>* pVCBNMap =
		new VertexConstantBuffer<cbCreateNormalMapInstanced>(gfx, normalMapData, 0u, 1u);
	pNormalMapVSDraw = pVCBNMap->GetVertexConstantBuffer();

	

	VertexConstantBuffer<ShadowMapGenVS>* pVCBSMGen =
		new VertexConstantBuffer<ShadowMapGenVS>(gfx, shadowMapCbuffer, 0u, 1u);
	pShadomMapGenCB = pVCBSMGen->GetVertexConstantBuffer();


	conePSCB.mat.diffuseAlbedo = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	conePSCB.mat.fresnelR0 = DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f);
	conePSCB.mat.shininess = 0.7f;
	conePSCB.dirLight.strength = DirectX::XMFLOAT3(0.8f, 0.8f, 0.8f);

	PixelShaderConstantBuffer<cbDefaultPS>* pLightsCB =
		new PixelShaderConstantBuffer<cbDefaultPS>(gfx, conePSCB, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pShadowMapConeDrawPS = pLightsCB->GetPixelShaderConstantBuffer();

	std::wstring directory[1];
	std::wstring normalMap[1];
	directory[0] = L"Textures\\bricks3.dds";
	normalMap[0] = L"Textures\\bricks3_nmap.dds";

	ShaderResourceView* pSRV = new ShaderResourceView(gfx, directory, 0u,  (UINT)std::size(directory), ShaderType::Pixel);
	AddBind(pSRV);

	ShaderResourceView* pSRVN = new ShaderResourceView(gfx, normalMap, 1u, 1u, ShaderType::Pixel);
	AddBind(pSRVN);

	TextureSampler* pTexSampler = new TextureSampler(gfx, ShaderType::Pixel);
	AddBind(pTexSampler);
}

void Cylinder::UpdateShadowMapGenBuffersInstanced(Graphics& gfx, const DirectX::XMMATRIX& in_lightView)
{
	gfx.pgfx_pDeviceContext->IASetVertexBuffers(0u, 2u, pIAbuffers, stride, offset);
	gfx.pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &pShadomMapGenCB);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pShadomMapGenCB, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	ShadowMapGenVS* pMatrices = reinterpret_cast<ShadowMapGenVS*>(mappedData.pData);
	pMatrices->lightWVP = DirectX::XMMatrixTranspose(in_lightView);
	pMatrices->texTransform = DirectX::XMMatrixIdentity();
	gfx.pgfx_pDeviceContext->Unmap(pShadomMapGenCB, 0u);
}


void Cylinder::UpdateDrawInstancedBuffers(Graphics& gfx, DirectX::XMFLOAT3 newCamPosition,
	const DirectX::XMMATRIX& newShadowTransform,
	const DirectX::XMMATRIX& in_ViewProj, ID3D11ShaderResourceView* pShadowMapSRV,
	DirectX::XMFLOAT3& newLightDirection)
{
	gfx.pgfx_pDeviceContext->IASetVertexBuffers(0u, 2u, pIAbuffers, stride, offset);


	D3D11_MAPPED_SUBRESOURCE mappedData;
	gfx.pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &pShadowMapVSDraw);
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pShadowMapVSDraw, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	cbDefaultVS* shadowVS = reinterpret_cast<cbDefaultVS*> (mappedData.pData);
	shadowVS->texTransform = DirectX::XMMatrixIdentity();
	shadowVS->shadowTransform = newShadowTransform;
	shadowVS->world = DirectX::XMMatrixIdentity();
	shadowVS->worldInvTranspose = DirectX::XMMatrixIdentity();
	shadowVS->viewProjection = DirectX::XMMatrixTranspose(in_ViewProj);
	shadowVS->matTransform = DirectX::XMMatrixIdentity();
	shadowVS->enableDisplacementMapping = false;
	shadowVS->cameraPositon = newCamPosition;
	gfx.pgfx_pDeviceContext->Unmap(pShadowMapVSDraw, 0u);


	gfx.pgfx_pDeviceContext->PSSetConstantBuffers(0u, 1u, &pShadowMapConeDrawPS);
	gfx.pgfx_pDeviceContext->PSSetShaderResources(2u, 1u, &pShadowMapSRV);
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pShadowMapConeDrawPS, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));

	cbDefaultPS* surface = reinterpret_cast<cbDefaultPS*> (mappedData.pData);
	surface->camPositon = newCamPosition;
	surface->lightDirection = newLightDirection;
	gfx.pgfx_pDeviceContext->Unmap(pShadowMapConeDrawPS, 0u);

}

void Cylinder::UpdateNormalMapBuffer(Graphics& gfx, const DirectX::XMMATRIX& in_ViewM,
	const DirectX::XMMATRIX& in_ViewProjection)
{
	gfx.pgfx_pDeviceContext->IASetVertexBuffers(0u, 2u, pIAbuffers, stride, offset);

	gfx.pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &pNormalMapVSDraw);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pNormalMapVSDraw, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	cbCreateNormalMapInstanced* nMap = reinterpret_cast<cbCreateNormalMapInstanced*> (mappedData.pData);
	nMap->view = DirectX::XMMatrixTranspose(in_ViewM);
	nMap->invTransposeView = MathHelper::InverseTranspose(in_ViewM);
	nMap->viewProjection = DirectX::XMMatrixTranspose(in_ViewProjection);
	gfx.pgfx_pDeviceContext->Unmap(pNormalMapVSDraw, 0u);
}

