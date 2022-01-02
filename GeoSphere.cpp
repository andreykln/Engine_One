#include "GeoSphere.h"
#include "App.h"

GeoSphere::GeoSphere(Graphics& gfx, float radius, UINT numSubdivisions, bool in_centerSphere, DemoSwitch in_switch)
	: centerSphere(in_centerSphere), currentDemo(in_switch)
{


	sphere.CreateGeosphere(radius, numSubdivisions, mesh);
	std::vector<Vertices_Full> vertices(mesh.vertices.size());
	for (UINT i = 0; i < mesh.vertices.size(); i++)
	{
		DirectX::XMFLOAT3 p = mesh.vertices[i].position;
		DirectX::XMFLOAT3 n = mesh.vertices[i].normal;
		DirectX::XMFLOAT2 t = mesh.vertices[i].TexC;
		if (std::isnan(t.x))
		{
			t = { 0.5f, 0.0f };
		}
		vertices[i].pos = p;
		vertices[i].normal = mesh.vertices[i].normal;
		vertices[i].tex = t;
		vertices[i].tangent = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	
	}

	//build world matrices for 10 spheres
	for (int i = 0; i < 5; ++i)
	{
		DirectX::XMStoreFloat4x4(&sGeoSphereWorld[i * 2 + 0],
			DirectX::XMMatrixTranslation(-5.0f, 3.5f, -10.0f + i * 5.0f));

		DirectX::XMStoreFloat4x4(&sGeoSphereWorld[i * 2 + 1],
			DirectX::XMMatrixTranslation(5.0f, 3.5f, -10.0f + i * 5.0f));
	}
	std::vector<DirectX::XMFLOAT4X4> geoSphereWorld(10);
	for (size_t i = 0; i < 10; i++)
	{
		geoSphereWorld[i] = sGeoSphereWorld[i];
	}



	VertexBufferInstancedDynamic* pVertexBuffer = new VertexBufferInstancedDynamic(gfx, vertices, geoSphereWorld, L"IstancedVB");
	pIAbuffers[0] = pVertexBuffer->GetVertexData();
	pIAbuffers[1] = pVertexBuffer->GetInstancedData();
	stride[0] = sizeof(Vertices_Full);
	stride[1] = sizeof(DirectX::XMFLOAT4X4);

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, mesh.indices, L"GeoSphereIndexBuffer");
	AddIndexBuffer(pIndexBuffer);


	VertexConstantBuffer<cbCreateNormalMapInstanced>* pVCBNMap =
		new VertexConstantBuffer<cbCreateNormalMapInstanced>(gfx, normalMapData, 0u, 1u);
	pNormalMapVSDraw = pVCBNMap->GetVertexConstantBuffer();


	VertexConstantBuffer<cbDefaultVS>* pVCBPerObject =
		new VertexConstantBuffer<cbDefaultVS>(gfx, geoSphereVSCB, 0u, 1u);
	pShadowMapVSDraw = pVCBPerObject->GetVertexConstantBuffer();
	VertexConstantBuffer<ShadowMapGenVS>* pVCBSMGen =
		new VertexConstantBuffer<ShadowMapGenVS>(gfx, shadowMapCbuffer, 0u, 1u);
	pShadomMapGenCB = pVCBSMGen->GetVertexConstantBuffer();


	geoSpherePSCB.mat.diffuseAlbedo = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	geoSpherePSCB.mat.fresnelR0 = DirectX::XMFLOAT3(0.98f, 0.97f, 0.95f);
	geoSpherePSCB.mat.shininess = 0.9f;
	geoSpherePSCB.dirLight.strength = DirectX::XMFLOAT3(0.8f, 0.8f, 0.8f);

	PixelShaderConstantBuffer<cbDefaultPS>* pLightsCB =
		new PixelShaderConstantBuffer<cbDefaultPS>(gfx, geoSpherePSCB, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pLightGeoSphere = pLightsCB->GetPixelShaderConstantBuffer();

	std::wstring directory[1];

	directory[0] = L"Textures\\stone.dds";
	ShaderResourceView* pSRV = new ShaderResourceView(gfx, directory, 0u, (UINT)std::size(directory), ShaderType::Pixel);
	AddBind(pSRV);

	TextureSampler* pTexSampler = new TextureSampler(gfx, ShaderType::Pixel);
	AddBind(pTexSampler);
}


void GeoSphere::UpdateShadowMapGenBuffersInstanced(Graphics& gfx, const DirectX::XMMATRIX& in_lightView)
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

void GeoSphere::UpdateShadowMapDrawInstancedBuffers(Graphics& gfx, DirectX::XMFLOAT3 newCamPosition, const DirectX::XMMATRIX& newShadowTransform, const DirectX::XMMATRIX& in_ViewProj, ID3D11ShaderResourceView* pShadowMapSRV, DirectX::XMFLOAT3& newLightDirection)
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
	shadowVS->cameraPositon = newCamPosition;
	shadowVS->enableDisplacementMapping = false;
	gfx.pgfx_pDeviceContext->Unmap(pShadowMapVSDraw, 0u);


	gfx.pgfx_pDeviceContext->PSSetConstantBuffers(0u, 1u, &pLightGeoSphere);
	gfx.pgfx_pDeviceContext->PSSetShaderResources(2u, 1u, &pShadowMapSRV);
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pLightGeoSphere, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));

	cbDefaultPS* surface = reinterpret_cast<cbDefaultPS*> (mappedData.pData);
	surface->disableTexSampling = true;

	surface->camPositon = newCamPosition;
	surface->lightDirection = newLightDirection;
	gfx.pgfx_pDeviceContext->Unmap(pLightGeoSphere, 0u);
}

void GeoSphere::UpdateNormalMapBuffer(Graphics& gfx, const DirectX::XMMATRIX& in_ViewM, const DirectX::XMMATRIX& in_ViewProjection)
{
	gfx.pgfx_pDeviceContext->IASetVertexBuffers(0u, 2u, pIAbuffers, stride, offset);

	gfx.pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &pNormalMapVSDraw);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pNormalMapVSDraw, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	cbCreateNormalMapInstanced* nMap = reinterpret_cast<cbCreateNormalMapInstanced*> (mappedData.pData);
	for (int i = 0; i < 10; i++)
	{
		nMap->worldInvTransposeView[i] = DirectX::XMMatrixTranspose(
			MathHelper::InverseTranspose(
				DirectX::XMLoadFloat4x4(&sGeoSphereWorld[i])) * in_ViewM);
		nMap->worldView[i] = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&sGeoSphereWorld[i]) * in_ViewM);
		nMap->worldViewProjection[i] = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&sGeoSphereWorld[i]) * in_ViewProjection);

	}
	gfx.pgfx_pDeviceContext->Unmap(pNormalMapVSDraw, 0u);
}

