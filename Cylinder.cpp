#include "Cylinder.h"
Cylinder::Cylinder(Graphics& gfx,
	float bottom_radius, float top_radius, float height, UINT slice_count, UINT stack_count, DemoSwitch in_switch)
	: currentDemo{in_switch}
{
	if (in_switch == DemoSwitch::LightningCone)
	{
		cylinderParts.CreateCylinderNoCaps(bottom_radius, top_radius, height, slice_count, stack_count, mesh);
	}
	if(in_switch == DemoSwitch::Shapesdemo || DemoSwitch::DisplacementMapping || DemoSwitch::ShadowMap)
	{
		cylinderParts.CreateCylinder(bottom_radius, top_radius, height, slice_count, stack_count, mesh);
	}
	std::vector<Vertex_IA> vertices(mesh.vertices.size());

	if (in_switch == DemoSwitch::LightningCone)
	{
		for (UINT i = 0; i < mesh.vertices.size(); i++)
		{
			DirectX::XMFLOAT3 p = mesh.vertices[i].position;
			DirectX::XMFLOAT3 n = mesh.vertices[i].normal;
			DirectX::XMFLOAT2 t = mesh.vertices[i].TexC;

			vertices[i].pos = p;
			vertices[i].normal = n;
			vertices[i].tex = t;
		}
	}
	std::vector<Vertices_Full> verticesWithNormals(mesh.vertices.size());

	if (in_switch == DemoSwitch::Shapesdemo || DemoSwitch::DisplacementMapping || DemoSwitch::ShadowMap)
	{
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
	}
	directionalLight.mat.ambient = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	directionalLight.mat.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	directionalLight.mat.specular = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);

	directionalLight.dirLight[0].ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	directionalLight.dirLight[0].diffuse = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	directionalLight.dirLight[0].direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	directionalLight.dirLight[0].specular = DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);

	directionalLight.dirLight[1].ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	directionalLight.dirLight[1].diffuse = DirectX::XMFLOAT4(0.55f, 0.55f, 0.55f, 1.0f);
	directionalLight.dirLight[1].direction = DirectX::XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);
	directionalLight.dirLight[1].specular = DirectX::XMFLOAT4(0.05f, 0.05f, 0.05f, 1.0f);

	directionalLight.dirLight[2].ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	directionalLight.dirLight[2].diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	directionalLight.dirLight[2].direction = DirectX::XMFLOAT3(0.0f, -0.707f, -0.707f);
	directionalLight.dirLight[2].specular = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	if (in_switch == DemoSwitch::LightningCone)
	{
		VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, vertices, L"Cylinder");
		AddBind(pVertexBuffer);
	}

	if (in_switch == DemoSwitch::Shapesdemo || DemoSwitch::DisplacementMapping || DemoSwitch::ShadowMap)
	{
		VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, verticesWithNormals, L"CylinderNormals");
		AddBind(pVertexBuffer);
	}

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, mesh.indices, L"CylinderIndexBuffer");
	AddIndexBuffer(pIndexBuffer);


	switch (currentDemo)
	{
	case Shapesdemo:
	{
		VertexConstantBuffer<CB_VS_Transform>* pVCBPerObject =
			new VertexConstantBuffer<CB_VS_Transform>(gfx, transformMatrices, 0u, 1u);
		pCopyLightTextureCylinder = pVCBPerObject->GetVertexConstantBuffer();
		AddBind(pVCBPerObject);
	}
		break;
	case DisplacementMapping:
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
		pCopyPCBLightsCylinder = pLightsCB->GetPixelShaderConstantBuffer();
		AddBind(pLightsCB);

	}


	if (currentDemo == DemoSwitch::DisplacementMapping)
	{
		DomainShaderConstantBuffer<CB_CameraPosition_ViewProj>* pDSCamPos =
			new DomainShaderConstantBuffer<CB_CameraPosition_ViewProj>(
				gfx, displacementMappingCylinderCB, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
		pCopyDomainShaderBuffer = pDSCamPos->GetDomainShaderConstantBuffer();
		AddBind(pDSCamPos);
	}
	//TODO delete?
	if (currentDemo == DemoSwitch::LightningCone)
	{
		std::wstring LightningArray[60];
		for (UINT i = 0; i < 60; ++i)
		{
			LightningArray[i] = L"Textures\\Lightning\\Spark" + std::to_wstring(i + 1) + L".dds";
		}

	}

	std::wstring directory[1];
	std::wstring normalMap[1];
	if (currentDemo == ShadowMap)
	{
		directory[0] = L"Textures\\bricks3.dds";
		normalMap[0] = L"Textures\\bricks3_nmap.dds";
	}
	else
	{
		directory[0] = L"Textures\\bricks.dds";
		normalMap[0] = L"Textures\\bricks_nmap.dds";
	}
	ShaderResourceView* pSRV = new ShaderResourceView(gfx, directory, 0u,  (UINT)std::size(directory), ShaderType::Pixel);
	AddBind(pSRV);

	ShaderResourceView* pSRVN = new ShaderResourceView(gfx, normalMap, 1u, 1u, ShaderType::Pixel);
	AddBind(pSRVN);
	if (currentDemo == DemoSwitch::DisplacementMapping)
	{
		ShaderResourceView* pSRVHeightMap = new ShaderResourceView(
			gfx, normalMap, 1u, (UINT)std::size(normalMap), ShaderType::Domain);
		AddBind(pSRVHeightMap);
	}

	

	TextureSampler* pTexSampler = new TextureSampler(gfx, ShaderType::Pixel);
	AddBind(pTexSampler);


	if (currentDemo == DemoSwitch::DisplacementMapping)
	{
		TextureSampler* pDomainTexSampler = new TextureSampler(gfx, ShaderType::Domain);
		AddBind(pDomainTexSampler);
	}


}

void Cylinder::DrawCylinder(Graphics& gfx, const DirectX::XMMATRIX& in_world,
	const DirectX::XMMATRIX& in_ViewProj, const DirectX::XMFLOAT3 camPositon)
{
	UpdateDisplacementCBuffers(gfx, in_world, in_ViewProj, camPositon);
	UpdatePSConstBuffers(gfx, camPositon);
	BindAndDrawIndexed(gfx);
}


void Cylinder::UpdateDisplacementCBuffers(Graphics& gfx, const DirectX::XMMATRIX& in_world,
	const DirectX::XMMATRIX& in_ViewProj, const DirectX::XMFLOAT3 in_camera)
{
	if (currentDemo == DisplacementMapping)
	{
		D3D11_MAPPED_SUBRESOURCE mappedData;
		DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBMatricesCylinder, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
		CB_VS_TransformWithCameraPosition* pMatrices = reinterpret_cast<CB_VS_TransformWithCameraPosition*>(mappedData.pData);
		pMatrices->world = DirectX::XMMatrixTranspose(in_world);
		pMatrices->worldInvTranspose = MathHelper::InverseTranspose(in_world);
		pMatrices->worldViewProjection = DirectX::XMMatrixTranspose(in_world * in_ViewProj);
		pMatrices->texTransform = DirectX::XMMatrixIdentity();
		pMatrices->cameraPosition = in_camera;
		gfx.pgfx_pDeviceContext->Unmap(pCopyVCBMatricesCylinder, 0u);

		DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyDomainShaderBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
		CB_CameraPosition_ViewProj* pDSBuffer = reinterpret_cast<CB_CameraPosition_ViewProj*>(mappedData.pData);
		pDSBuffer->cameraPosition = in_camera;
		pDSBuffer->viewProjection = DirectX::XMMatrixTranspose(in_ViewProj);
		gfx.pgfx_pDeviceContext->Unmap(pCopyDomainShaderBuffer, 0u);
	}
	if (currentDemo == ShadowMap)
	{
		D3D11_MAPPED_SUBRESOURCE mappedData;
		DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBMatricesCylinder, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
		CB_VS_TransformWithCameraPosition* pMatrices = reinterpret_cast<CB_VS_TransformWithCameraPosition*>(mappedData.pData);
		pMatrices->world = DirectX::XMMatrixTranspose(in_world);
		pMatrices->worldInvTranspose = MathHelper::InverseTranspose(in_world);
		pMatrices->worldViewProjection = DirectX::XMMatrixTranspose(in_world * in_ViewProj);
		pMatrices->texTransform = DirectX::XMMatrixIdentity();
		pMatrices->cameraPosition = in_camera;
		gfx.pgfx_pDeviceContext->Unmap(pCopyVCBMatricesCylinder, 0u);
	}

}

void Cylinder::UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyLightTextureCylinder, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CB_VS_Transform* pMatrices = reinterpret_cast<CB_VS_Transform*>(mappedData.pData);
	pMatrices->world = in_world;
	pMatrices->worldInvTranspose = MathHelper::InverseTranspose(in_world);
	pMatrices->worldViewProjection = DirectX::XMMatrixTranspose(in_world * in_ViewProj);
	pMatrices->texTransform = DirectX::XMMatrixIdentity();
	gfx.pgfx_pDeviceContext->Unmap(pCopyLightTextureCylinder, 0u);

}

void Cylinder::UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPositon)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPCBLightsCylinder, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CB_PS_PerFrameUpdate* frame = reinterpret_cast<CB_PS_PerFrameUpdate*> (mappedData.pData);
	frame->cameraPositon = camPositon;

	if (currentDemo == DemoSwitch::LightningCone)
	{
		frame->texArrayPos = GetTexArrPos();
	}


	if (GetAsyncKeyState('0') & 0x8000)
		frame->numberOfLights = 0;
	if (GetAsyncKeyState('1') & 0x8000)
		frame->numberOfLights = 1;
	if (GetAsyncKeyState('2') & 0x8000)
		frame->numberOfLights = 2;
	if (GetAsyncKeyState('3') & 0x8000)
		frame->numberOfLights = 3;

	gfx.pgfx_pDeviceContext->Unmap(pCopyPCBLightsCylinder, 0u);
}

void Cylinder::IncrementTexArrPos() noexcept
{
	texArrPosition++;
	if (texArrPosition > 60)
	{
		texArrPosition = 0;
	}
}

UINT Cylinder::GetTexArrPos() const noexcept
{
	return texArrPosition;
}

void Cylinder::UpdateShadomMapGenBuffers(Graphics& gfx, const DirectX::XMMATRIX& in_lightWorld, DirectX::XMFLOAT3 newCamPosition)
{
	gfx.pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &pShadomMapGenCB);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pShadomMapGenCB, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	ShadowMapGenVS* pMatrices = reinterpret_cast<ShadowMapGenVS*>(mappedData.pData);
	pMatrices->lightWVP = DirectX::XMMatrixTranspose(in_lightWorld);
	pMatrices->texTransform = DirectX::XMMatrixIdentity();
	gfx.pgfx_pDeviceContext->Unmap(pShadomMapGenCB, 0u);
}

void Cylinder::UpdateShadowMapDrawBuffers(Graphics& gfx, DirectX::XMFLOAT3 newCamPosition, const DirectX::XMMATRIX& newShadowTransform,
	const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj, ID3D11ShaderResourceView* pShadowMapSRV,
	DirectX::XMFLOAT3* newLightDirection)
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



DirectionalLight* Cylinder::GetOldLightDirection()
{
	return directionalLight.dirLight;
}

