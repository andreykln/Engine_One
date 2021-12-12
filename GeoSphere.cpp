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


	switch (currentDemo)
	{

	case ShadowMap:
	{
		VertexConstantBuffer<cbDefaultVS>* pVCBPerObject =
			new VertexConstantBuffer<cbDefaultVS>(gfx, geoSphereVSCB, 0u, 1u);
		pShadowMapVSDraw = pVCBPerObject->GetVertexConstantBuffer();
		VertexConstantBuffer<ShadowMapGenVS>* pVCBSMGen =
			new VertexConstantBuffer<ShadowMapGenVS>(gfx, shadowMapCbuffer, 0u, 1u);
		pShadomMapGenCB = pVCBSMGen->GetVertexConstantBuffer();
	}
		break;
	default:
	{
		VertexConstantBuffer<CB_VS_Transform>* pVCBPerObject =
			new VertexConstantBuffer<CB_VS_Transform>(gfx, transformMatrices, 0u, 1u);
		pCopyVCBMatricesGeoSphere = pVCBPerObject->GetVertexConstantBuffer(); //for updating every frame
		AddBind(pVCBPerObject);
	}
		break;
	}



	geoSpherePSCB.mat.diffuseAlbedo = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	geoSpherePSCB.mat.fresnelR0 = DirectX::XMFLOAT3(0.98f, 0.97f, 0.95f);
	geoSpherePSCB.mat.shininess = 0.9f;
	geoSpherePSCB.dirLight.strength = DirectX::XMFLOAT3(0.8f, 0.8f, 0.8f);

	if (currentDemo == ShadowMap)
	{
		PixelShaderConstantBuffer<cbDefaultPS>* pLightsCB =
			new PixelShaderConstantBuffer<cbDefaultPS>(gfx, geoSpherePSCB, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
		pLightGeoSphere = pLightsCB->GetPixelShaderConstantBuffer();
	}
	else
	{
		PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>* pLightsCB =
			new PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>(gfx, pscBuffer, 1u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
		pCopyPCBLightsGeoSphere = pLightsCB->GetPixelShaderConstantBuffer();
		AddBind(pLightsCB);
	}





	std::wstring directory[1];

	if (centerSphere)
	{

	}
	if (!centerSphere)
	{
		directory[0] = L"Textures\\stone.dds";
		ShaderResourceView* pSRV = new ShaderResourceView(gfx, directory, 0u, (UINT)std::size(directory), ShaderType::Pixel);
		AddBind(pSRV);

	}



	TextureSampler* pTexSampler = new TextureSampler(gfx, ShaderType::Pixel);
	AddBind(pTexSampler);
}

void GeoSphere::DrawSpheres(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj, float dt, DirectX::XMFLOAT3 camPositon)
{
	UpdateVSMatrices(gfx, in_world, in_ViewProj, sin(dt));
	UpdatePSConstBuffers(gfx, camPositon);
	BindAndDrawIndexed(gfx);
}

void GeoSphere::UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj, float dt)
{
	sphereTextureOffset.y += 0.05f * dt;
	sphereTextureOffset.x += 0.1f * dt;
	sphereOffset = DirectX::XMMatrixTranslation(0.0f, sphereTextureOffset.x, sphereTextureOffset.y);



	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBMatricesGeoSphere, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CB_VS_Transform* pMatrices = reinterpret_cast<CB_VS_Transform*>(mappedData.pData);
	pMatrices->world = in_world;
	pMatrices->worldInvTranspose = MathHelper::InverseTranspose(in_world);
	pMatrices->worldViewProjection = DirectX::XMMatrixTranspose(in_world * in_ViewProj);
	pMatrices->texTransform = DirectX::XMMatrixIdentity();
	gfx.pgfx_pDeviceContext->Unmap(pCopyVCBMatricesGeoSphere, 0u);
}

void GeoSphere::UpdateShadomMapGenBuffers(Graphics& gfx, const DirectX::XMMATRIX& in_lightWorld, DirectX::XMFLOAT3 newCamPosition)
{
	gfx.pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &pShadomMapGenCB);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pShadomMapGenCB, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	ShadowMapGenVS* pMatrices = reinterpret_cast<ShadowMapGenVS*>(mappedData.pData);
	pMatrices->lightWVP = DirectX::XMMatrixTranspose(in_lightWorld);
	pMatrices->texTransform = DirectX::XMMatrixIdentity();
	gfx.pgfx_pDeviceContext->Unmap(pShadomMapGenCB, 0u);
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

void GeoSphere::UpdateShadowMapDrawBuffers(Graphics& gfx, DirectX::XMFLOAT3 newCamPosition,
	const DirectX::XMMATRIX& newShadowTransform, const DirectX::XMMATRIX& in_world,
	const DirectX::XMMATRIX& in_ViewProj, ID3D11ShaderResourceView* pShadowMapSRV, DirectX::XMFLOAT3& newLightDirection)
{
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

void GeoSphere::UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPositon)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPCBLightsGeoSphere, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CB_PS_PerFrameUpdate* frame = reinterpret_cast<CB_PS_PerFrameUpdate*> (mappedData.pData);
	frame->cameraPositon = camPositon;

	if (GetAsyncKeyState('0') & 0x8000)
		frame->numberOfLights = 0;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
	if (GetAsyncKeyState('1') & 0x8000)
		frame->numberOfLights = 1;
	if (GetAsyncKeyState('2') & 0x8000)
		frame->numberOfLights = 2;
	if (GetAsyncKeyState('3') & 0x8000)
		frame->numberOfLights = 3;

	gfx.pgfx_pDeviceContext->Unmap(pCopyPCBLightsGeoSphere, 0u);
}

