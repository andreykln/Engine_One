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
	case DemoSwitch::Shapesdemo:
	case DemoSwitch::DefaultBox:
	case DemoSwitch::DisplacementMapping:
	case DemoSwitch::ShadowMap:
	{
		directionalLight.dirLight[0].ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		directionalLight.dirLight[0].diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		directionalLight.dirLight[0].direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
		directionalLight.dirLight[0].specular = DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);

		directionalLight.dirLight[1].ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
		directionalLight.dirLight[1].diffuse = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
		directionalLight.dirLight[1].direction = DirectX::XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);
		directionalLight.dirLight[1].specular = DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);

		directionalLight.dirLight[2].ambient = DirectX::XMFLOAT4(0.5, 0.5f, 0.5f, 1.0f);
		directionalLight.dirLight[2].diffuse = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
		directionalLight.dirLight[2].direction = DirectX::XMFLOAT3(0.0f, -0.707f, -0.707f);
		directionalLight.dirLight[2].specular = DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
		directionalLight.mat.ambient = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		directionalLight.mat.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		directionalLight.mat.specular = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 16.0f);
		directionalLight.mat.reflect = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	break;
	case DemoSwitch::HillsDemo:
	{
		directionalLight.dirLight[0].ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		directionalLight.dirLight[0].diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		directionalLight.dirLight[0].direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
		directionalLight.dirLight[0].specular = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);

		directionalLight.dirLight[1].ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
		directionalLight.dirLight[1].diffuse = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
		directionalLight.dirLight[1].direction = DirectX::XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);
		directionalLight.dirLight[1].specular = DirectX::XMFLOAT4(0.55f, 0.55f, 0.55f, 1.0f);

		directionalLight.dirLight[2].ambient = DirectX::XMFLOAT4(0.5, 0.5f, 0.5f, 1.0f);
		directionalLight.dirLight[2].diffuse = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
		directionalLight.dirLight[2].direction = DirectX::XMFLOAT3(0.0f, -0.707f, -0.707f);
		directionalLight.dirLight[2].specular = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
		directionalLight.mat.ambient = DirectX::XMFLOAT4(0.984f, 0.565f, 0.384f, 1.0f);
		directionalLight.mat.diffuse = DirectX::XMFLOAT4(0.984f, 0.565f, 0.384f, 1.0f);
		directionalLight.mat.specular = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		directionalLight.mat.reflect = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	break;
	}

	switch (currentDemo)
	{
	case Shapesdemo:
	case DisplacementMapping:
	case ShadowMap:
	{
		VertexBuffer* pVB = new VertexBuffer(gfx, verticesNormal, L"BoxNormal.");
		AddBind(pVB);
	}
		break;
	case HillsDemo:
	case HillsAllLight:
	case DefaultBox:
	{
		Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		AddBind(pTopology);
		VertexBuffer* pVB = new VertexBuffer(gfx, vertices, L"Box.");
		AddBind(pVB);
	}
		break;
	default:
		break;
	}


	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, mesh.indices, L"BoxIndexBuffer.");
	AddIndexBuffer(pIndexBuffer);

	switch (currentDemo)
	{
	case DisplacementMapping:
	{
		VertexConstantBuffer<CB_VS_TransformWithCameraPosition>* pVSCB =
			new VertexConstantBuffer<CB_VS_TransformWithCameraPosition>(gfx, displacementMappingVS_CB, 0u, 1u);
		pCopyVSdisMappingCb = pVSCB->GetVertexConstantBuffer();
		AddBind(pVSCB);
		DomainShaderConstantBuffer<CB_CameraPosition_ViewProj>* pDSCamPos =
			new DomainShaderConstantBuffer<CB_CameraPosition_ViewProj>(
				gfx, displacementMappingDS_CB, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
		pCopyDSdisMappingCB = pDSCamPos->GetDomainShaderConstantBuffer();
		AddBind(pDSCamPos);
		break;
	}
	case ShadowMap:
	{
		VertexConstantBuffer<cbDefaultVS>* pVCBPerObject =
			new VertexConstantBuffer<cbDefaultVS>(gfx, boxVSCB, 0u, 1u);
		pShadowMapVSDraw = pVCBPerObject->GetVertexConstantBuffer();
		VertexConstantBuffer<ShadowMapGenVS>* pVCBSMGen =
			new VertexConstantBuffer<ShadowMapGenVS>(gfx, shadowMapCbuffer, 0u, 1u);
		pShadomMapGenCB = pVCBSMGen->GetVertexConstantBuffer();
		break;
	}
	default:
	{
		VertexConstantBuffer<CB_VS_Transform>* pVCBPerObject =
			new VertexConstantBuffer<CB_VS_Transform>(gfx, transformMatrices, 0u, 1u);
		pCopyVCBMatricesBox = pVCBPerObject->GetVertexConstantBuffer();
		AddBind(pVCBPerObject);
		break;
	}
	}

// 
// 	PixelShaderConstantBuffer<CB_PS_DirectionalL_Fog>* pPSCBPerFrame =
// 		new PixelShaderConstantBuffer<CB_PS_DirectionalL_Fog>(gfx, directionalLight, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
// 	pCopyLightPerFrame = pPSCBPerFrame->GetPixelShaderConstantBuffer();
// 	AddBind(pPSCBPerFrame);

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
		PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>* pLightsCB =
			new PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>(gfx, pscBuffer, 1u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
		pCopyPCBLightsBox = pLightsCB->GetPixelShaderConstantBuffer();
		AddBind(pLightsCB);
	}




	std::wstring directory[1];
	std::wstring normalMap[1];

	switch (currentDemo)
	{
	case DemoSwitch::Shapesdemo:
	case DemoSwitch::DisplacementMapping:
	{
		directory[0] = L"Textures\\bricks.dds";
		normalMap[0] = L"Textures\\bricks_nmap.dds";
		ShaderResourceView* pSRVHeightMap = new ShaderResourceView(
			gfx, normalMap, 1u, (UINT)std::size(normalMap), ShaderType::Domain);
		AddBind(pSRVHeightMap);
		ShaderResourceView* pSRVN = new ShaderResourceView(gfx, normalMap, 1u, 1u, ShaderType::Pixel);
		AddBind(pSRVN);
		break;
	}
	case DemoSwitch::ShadowMap:
	{
		directory[0] = L"Textures\\bricks3.dds";
		normalMap[0] = L"Textures\\bricks3_nmap.dds";
		ShaderResourceView* pSRVN = new ShaderResourceView(gfx, normalMap, 1u, 1u, ShaderType::Pixel);
		AddBind(pSRVN);
		break;
	}
	break;
	case DemoSwitch::HillsAllLight:
	case DemoSwitch::HillsDemo:
	{
		directory[0] = L"Textures\\WireFence.dds";
	}
	break;
	case DemoSwitch::DefaultBox:
	{
		directory[0] = L"Textures\\WoodCrate01.dds";
	}
	break;
	default:
		break;
	}



	ShaderResourceView* pSRV = new ShaderResourceView(gfx, directory, 0u, (UINT)std::size(directory), ShaderType::Pixel);
	AddBind(pSRV);


	TextureSampler* pTexSampler = new TextureSampler(gfx, ShaderType::Pixel);
	AddBind(pTexSampler);

	if (currentDemo == DemoSwitch::DisplacementMapping)
	{
		TextureSampler* pDomainTexSampler = new TextureSampler(gfx, ShaderType::Domain);
		AddBind(pDomainTexSampler);
	}

}


void Box::DrawBox(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj, DirectX::XMFLOAT3 camPositon)
{
	UpdateDisplacementCBuffers(gfx,	in_world, in_ViewProj, camPositon);
	UpdatePSConstBuffers(gfx, camPositon);
	BindAndDrawIndexed(gfx);
}

DirectX::XMMATRIX Box::GetBoxForHillsOffset()
{
	return boxforHillsOffset;
}

void Box::UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBMatricesBox, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CB_VS_Transform* pMatrices = reinterpret_cast<CB_VS_Transform*>(mappedData.pData);
	pMatrices->world = in_world;
	pMatrices->worldInvTranspose = MathHelper::InverseTranspose(in_world);
	pMatrices->worldViewProjection = DirectX::XMMatrixTranspose(in_world * in_ViewProj);
	pMatrices->texTransform = DirectX::XMMatrixIdentity();
	gfx.pgfx_pDeviceContext->Unmap(pCopyVCBMatricesBox, 0u);

}

void Box::UpdateDisplacementCBuffers(Graphics& gfx, const DirectX::XMMATRIX& in_world,
	const DirectX::XMMATRIX& in_ViewProj, const DirectX::XMFLOAT3 in_camera)
{
	switch (currentDemo)
	{
	case DisplacementMapping:
	{
		D3D11_MAPPED_SUBRESOURCE mappedData;
		DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVSdisMappingCb, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
		CB_VS_TransformWithCameraPosition* pMatrices = reinterpret_cast<CB_VS_TransformWithCameraPosition*>(mappedData.pData);
		pMatrices->world = DirectX::XMMatrixTranspose(in_world);
		pMatrices->worldInvTranspose = MathHelper::InverseTranspose(in_world);
		pMatrices->worldViewProjection = DirectX::XMMatrixTranspose(in_world * in_ViewProj);
		pMatrices->texTransform = DirectX::XMMatrixIdentity();
		pMatrices->cameraPosition = in_camera;
		gfx.pgfx_pDeviceContext->Unmap(pCopyVSdisMappingCb, 0u);

		DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyDSdisMappingCB, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
		CB_CameraPosition_ViewProj* pDSBuffer = reinterpret_cast<CB_CameraPosition_ViewProj*>(mappedData.pData);
		pDSBuffer->cameraPosition = in_camera;
		pDSBuffer->viewProjection = DirectX::XMMatrixTranspose(in_ViewProj);
		gfx.pgfx_pDeviceContext->Unmap(pCopyDSdisMappingCB, 0u);
		break;
	}
	case ShadowMap:
	{
		D3D11_MAPPED_SUBRESOURCE mappedData;
		DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVSdisMappingCb, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
		CB_VS_TransformWithCameraPosition* pMatrices = reinterpret_cast<CB_VS_TransformWithCameraPosition*>(mappedData.pData);
		pMatrices->world = DirectX::XMMatrixTranspose(in_world);
		pMatrices->worldInvTranspose = MathHelper::InverseTranspose(in_world);
		pMatrices->worldViewProjection = DirectX::XMMatrixTranspose(in_world * in_ViewProj);
		pMatrices->texTransform = DirectX::XMMatrixIdentity();
		pMatrices->cameraPosition = in_camera;
		gfx.pgfx_pDeviceContext->Unmap(pCopyVSdisMappingCb, 0u);
		break;
	}
	}

}

void Box::UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPositon)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPCBLightsBox, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
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

	gfx.pgfx_pDeviceContext->Unmap(pCopyPCBLightsBox, 0u);

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


	gfx.pgfx_pDeviceContext->PSSetConstantBuffers(0u, 1u, &pShadowMapBoxDrawPS);
	gfx.pgfx_pDeviceContext->PSSetShaderResources(2u, 1u, &pShadowMapSRV);
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pShadowMapBoxDrawPS, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));

	cbDefaultPS* surface = reinterpret_cast<cbDefaultPS*> (mappedData.pData);
	surface->camPositon = newCamPosition;
	surface->lightDirection = newLightDirection;
	gfx.pgfx_pDeviceContext->Unmap(pShadowMapBoxDrawPS, 0u);
}

