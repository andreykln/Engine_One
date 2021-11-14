#include "GeoSphere.h"
#include "App.h"

GeoSphere::GeoSphere(Graphics& gfx, float radius, UINT numSubdivisions, bool in_centerSphere, DemoSwitch in_switch)
	: centerSphere(in_centerSphere), currentDemo(in_switch)
{


	directionalLight.dirLight[0].ambient = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	directionalLight.dirLight[0].diffuse = DirectX::XMFLOAT4(0.75f, 0.75f, 0.75f, 1.0f);
	directionalLight.dirLight[0].direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	directionalLight.dirLight[0].specular = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	directionalLight.dirLight[1].ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	directionalLight.dirLight[1].diffuse = DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	directionalLight.dirLight[1].direction = DirectX::XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);
	directionalLight.dirLight[1].specular = DirectX::XMFLOAT4(0.35f, 0.35f, 0.35f, 1.0f);
	directionalLight.dirLight[2].ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	directionalLight.dirLight[2].diffuse = DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	directionalLight.dirLight[2].direction = DirectX::XMFLOAT3(0.0f, -0.707f, -0.707f);
	directionalLight.dirLight[2].specular = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);

	if (centerSphere)
	{
		directionalLight.mat.ambient = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		directionalLight.mat.diffuse = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		directionalLight.mat.specular = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);
		directionalLight.mat.reflect = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);

	}
	else
	{
		directionalLight.mat.ambient = DirectX::XMFLOAT4(0.6f, 0.8f, 1.0f, 1.0f);
		directionalLight.mat.diffuse = DirectX::XMFLOAT4(0.6f, 0.8f, 1.0f, 1.0f);
		directionalLight.mat.specular = DirectX::XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f);
		directionalLight.mat.reflect = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	}

	sphere.CreateGeosphere(radius, numSubdivisions, mesh);
	std::vector<Vertex_IA> vertices(mesh.vertices.size());
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
	}


	VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, vertices, L"GeoSphere");
	AddBind(pVertexBuffer);

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, mesh.indices, L"GeoSphereIndexBuffer");
	AddIndexBuffer(pIndexBuffer);


	switch (currentDemo)
	{

	case ShadowMap:
	{
		VertexConstantBuffer<CB_VS_ShadowMapDraw>* pVCBPerObject =
			new VertexConstantBuffer<CB_VS_ShadowMapDraw>(gfx, shadowMapVSDraw, 0u, 1u);
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



	PixelShaderConstantBuffer<CB_PS_DirectionalL_Fog>* pLightsPS =
		new PixelShaderConstantBuffer<CB_PS_DirectionalL_Fog>(gfx, directionalLight, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pLightDirectionPSCbuffer = pLightsPS->GetPixelShaderConstantBuffer();
	AddBind(pLightsPS);


	if (currentDemo == ShadowMap)
	{
		PixelShaderConstantBuffer<CB_PS_ShadowMapDraw>* pLightsCB =
			new PixelShaderConstantBuffer<CB_PS_ShadowMapDraw>(gfx, shadowMapDraw, 1u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
		pCopyPCBLightsCylinder = pLightsCB->GetPixelShaderConstantBuffer();
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

void GeoSphere::UpdateShadowMapDrawBuffers(Graphics& gfx, DirectX::XMFLOAT3 newCamPosition, const DirectX::XMMATRIX& newShadowTransform, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj, ID3D11ShaderResourceView* pShadowMapSRV, DirectX::XMFLOAT3* newLightDirection)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	gfx.pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &pShadowMapVSDraw);
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pShadowMapVSDraw, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CB_VS_ShadowMapDraw* shadowVS = reinterpret_cast<CB_VS_ShadowMapDraw*> (mappedData.pData);
	shadowVS->texTransform = DirectX::XMMatrixIdentity();
	shadowVS->shadowTransform = newShadowTransform;
	shadowVS->world = in_world;
	shadowVS->worldInvTranspose = MathHelper::InverseTranspose(in_world);
	shadowVS->worldViewProjection = DirectX::XMMatrixTranspose(in_world * in_ViewProj);
	gfx.pgfx_pDeviceContext->Unmap(pShadowMapVSDraw, 0u);


	gfx.pgfx_pDeviceContext->PSSetConstantBuffers(1u, 1u, &pCopyPCBLightsCylinder);
	gfx.pgfx_pDeviceContext->PSSetShaderResources(2u, 1u, &pShadowMapSRV);
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPCBLightsCylinder, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CB_PS_ShadowMapDraw* frame = reinterpret_cast<CB_PS_ShadowMapDraw*> (mappedData.pData);
	frame->cameraPositon = newCamPosition;
	frame->lightDirection = directionalLight.dirLight[0].direction;

	gfx.pgfx_pDeviceContext->Unmap(pCopyPCBLightsCylinder, 0u);


	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pLightDirectionPSCbuffer, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CB_PS_DirectionalL_Fog* lightDir = reinterpret_cast<CB_PS_DirectionalL_Fog*> (mappedData.pData);
	for (int i = 0; i < 3; i++)
	{
		lightDir->dirLight[i].direction = newLightDirection[i];
	}

	gfx.pgfx_pDeviceContext->Unmap(pLightDirectionPSCbuffer, 0u);
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

