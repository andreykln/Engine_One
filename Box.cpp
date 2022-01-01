#include "Box.h"
#include "MathHelper.h"


Box::Box(Graphics& gfx, float width, float height, float depth, DemoSwitch demo)
	: currentDemo{ demo }
{

	box.CreateBox(width, height, depth, mesh);
	std::vector<Vertex_IA> vertices(mesh.vertices.size());

	for (UINT i = 0; i < mesh.vertices.size(); i++)
	{
		DirectX::XMFLOAT3 p = mesh.vertices[i].position;
		DirectX::XMFLOAT3 n = mesh.vertices[i].normal;
		DirectX::XMFLOAT2 t = mesh.vertices[i].TexC;

		vertices[i].pos = p;
 		vertices[i].normal = n;
		vertices[i].tex = t;
	}

	std::vector<Vertices_Full> verticesNormal(mesh.vertices.size());
	for (UINT i = 0; i < mesh.vertices.size(); i++)
	{
		DirectX::XMFLOAT3 p = mesh.vertices[i].position;
		DirectX::XMFLOAT3 n = mesh.vertices[i].normal;
		DirectX::XMFLOAT2 t = mesh.vertices[i].TexC;
		DirectX::XMFLOAT3 tg = mesh.vertices[i].tangentU;

		verticesNormal[i].pos = p;
		verticesNormal[i].normal = n;
		verticesNormal[i].tex = t;
		verticesNormal[i].tangent = tg;
	}

	switch (currentDemo)
	{
	case ShadowMap:
	{
		VertexBuffer* pVB = new VertexBuffer(gfx, verticesNormal, L"BoxNormal.");
		AddBind(pVB);
		break;
	}
	case DefaultBox:
	{
// 		Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
// 		AddBind(pTopology);
// 		VertexBuffer* pVB = new VertexBuffer(gfx, vertices, L"Box.");
// 		AddBind(pVB);
// 		break;
	}
	}

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, mesh.indices, L"BoxIndexBuffer.");
	AddIndexBuffer(pIndexBuffer);

	switch (currentDemo)
	{
	case ShadowMap:
	{
		VertexConstantBuffer<cbDefaultVS>* pVCBPerObject =
			new VertexConstantBuffer<cbDefaultVS>(gfx, boxVSCB, 0u, 1u);
		pShadowMapVSDraw = pVCBPerObject->GetVertexConstantBuffer();
		//copy for normal map so it won't occupy the same memory causing flickering
		VertexConstantBuffer<cbCreateNormalMap>* pVCBNormalMap =
			new VertexConstantBuffer<cbCreateNormalMap>(gfx, normapMapData, 0u, 1u);
		pNormalMapVSDraw = pVCBNormalMap->GetVertexConstantBuffer();


		

		VertexConstantBuffer<ShadowMapGenVS>* pVCBSMGen =
			new VertexConstantBuffer<ShadowMapGenVS>(gfx, shadowMapCbuffer, 0u, 1u);
		pShadomMapGenCB = pVCBSMGen->GetVertexConstantBuffer();
		break;
	}
	default:
	{
// 		VertexConstantBuffer<CB_VS_Transform>* pVCBPerObject =
// 			new VertexConstantBuffer<CB_VS_Transform>(gfx, transformMatrices, 0u, 1u);
// 		pCopyVCBMatricesBox = pVCBPerObject->GetVertexConstantBuffer();
// 		AddBind(pVCBPerObject);
// 		break;
	}
	}

	boxPSCB.mat.diffuseAlbedo = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	boxPSCB.mat.fresnelR0 = DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f);
	boxPSCB.mat.shininess = 0.7f;
 	boxPSCB.dirLight.strength = DirectX::XMFLOAT3(0.8f, 0.8f, 0.8f);
	if (currentDemo == ShadowMap)
	{
		PixelShaderConstantBuffer<cbDefaultPS>* pLightsCB =
			new PixelShaderConstantBuffer<cbDefaultPS>(gfx, boxPSCB, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
		pShadowMapBoxDrawPS = pLightsCB->GetPixelShaderConstantBuffer();

	}
	else
	{
// 		PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>* pLightsCB =
// 			new PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>(gfx, pscBuffer, 1u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
// 		pCopyPCBLightsBox = pLightsCB->GetPixelShaderConstantBuffer();
// 		AddBind(pLightsCB);
	}

	std::wstring directory[1];

	switch (currentDemo)
	{

	case DemoSwitch::ShadowMap:
	{
		//directory[0] = L"Textures\\bricks3.dds";
		const wchar_t* diffuseMap = L"Textures\\bricks3.dds";
		const wchar_t* path = L"Textures\\bricks3_nmap.dds";
		ShaderResourceView* pSRVN = new ShaderResourceView(gfx, path);
		pNormalMap = pSRVN->GetSRV();

		ShaderResourceView* pSRV = new ShaderResourceView(gfx, diffuseMap);
		pDiffMapHeightMap = pSRV->GetSRV();
		break;
	}
	case DemoSwitch::DefaultBox:
	{
		const wchar_t* diffuseMap = L"Textures\\WoodCrate01.dds";
	}
	break;
	}


	TextureSampler* pTexSampler = new TextureSampler(gfx, ShaderType::Pixel);
	pSamplerState = pTexSampler->GetSamplerState();

}

void Box::UpdateShadomMapGenBuffers(Graphics& gfx, const DirectX::XMMATRIX& in_lightWorld, DirectX::XMFLOAT3 newCamPosition)
{
	gfx.pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &pShadomMapGenCB);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pShadomMapGenCB, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	ShadowMapGenVS* pMatrices = reinterpret_cast<ShadowMapGenVS*>(mappedData.pData);
	pMatrices->lightWVP = DirectX::XMMatrixTranspose(in_lightWorld);
	pMatrices->texTransform = DirectX::XMMatrixIdentity();
	gfx.pgfx_pDeviceContext->Unmap(pShadomMapGenCB, 0u);

}

void Box::UpdateShadowMapDrawBuffers(Graphics& gfx, DirectX::XMFLOAT3 newCamPosition,
	const DirectX::XMMATRIX& newShadowTransform, const DirectX::XMMATRIX& in_world,
	const DirectX::XMMATRIX& in_ViewProj, ID3D11ShaderResourceView* pShadowMapSRV, DirectX::XMFLOAT3& newLightDirection)
{
	gfx.pgfx_pDeviceContext->DSSetSamplers(0u, 1u, &pSamplerState);
	gfx.pgfx_pDeviceContext->PSSetSamplers(0u, 1u, &pSamplerState);
	gfx.pgfx_pDeviceContext->PSSetShaderResources(0u, 1u, &pDiffMapHeightMap);
	gfx.pgfx_pDeviceContext->PSSetShaderResources(1u, 1u, &pNormalMap);
	gfx.pgfx_pDeviceContext->DSSetShaderResources(0u, 1u, &pNormalMap);
	gfx.pgfx_pDeviceContext->DSSetConstantBuffers(0u, 1u, &pShadowMapVSDraw);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	gfx.pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &pShadowMapVSDraw);
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pShadowMapVSDraw, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	cbDefaultVS* shadowVS = reinterpret_cast<cbDefaultVS*> (mappedData.pData);
	shadowVS->texTransform = DirectX::XMMatrixIdentity();
	shadowVS->shadowTransform = newShadowTransform;
	shadowVS->world = DirectX::XMMatrixTranspose(in_world);
	shadowVS->worldInvTranspose = MathHelper::InverseTranspose(in_world);
	shadowVS->viewProjection = DirectX::XMMatrixTranspose(in_ViewProj);
	shadowVS->matTransform = DirectX::XMMatrixIdentity();
	shadowVS->enableDisplacementMapping = true;
	shadowVS->cameraPositon = newCamPosition;
	gfx.pgfx_pDeviceContext->Unmap(pShadowMapVSDraw, 0u);


	gfx.pgfx_pDeviceContext->PSSetConstantBuffers(0u, 1u, &pShadowMapBoxDrawPS);
	gfx.pgfx_pDeviceContext->PSSetShaderResources(2u, 1u, &pShadowMapSRV);
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pShadowMapBoxDrawPS, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));

	cbDefaultPS* surface = reinterpret_cast<cbDefaultPS*> (mappedData.pData);
	surface->camPositon = newCamPosition;
	surface->lightDirection = newLightDirection;
	gfx.pgfx_pDeviceContext->Unmap(pShadowMapBoxDrawPS, 0u);
}

void Box::UpdateNormalMapBuffer(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewM,
	const DirectX::XMMATRIX& in_ViewProjection)
{
	gfx.pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &pNormalMapVSDraw);


	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pNormalMapVSDraw, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	cbCreateNormalMap* nMap = reinterpret_cast<cbCreateNormalMap*> (mappedData.pData);
	nMap->worldInvTransposeView = (MathHelper::InverseTranspose(in_world) * DirectX::XMMatrixTranspose(in_ViewM));
	nMap->worldView = DirectX::XMMatrixTranspose(in_world * in_ViewM);
	nMap->worldViewProjection = DirectX::XMMatrixTranspose(in_world * in_ViewProjection);
	gfx.pgfx_pDeviceContext->Unmap(pNormalMapVSDraw, 0u);

}

