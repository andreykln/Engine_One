#include "Cylinder.h"
Cylinder::Cylinder(Graphics& gfx,
	float bottom_radius, float top_radius, float height, UINT slice_count, UINT stack_count, DemoSwitch in_switch)
	: currentDemo{in_switch}
{
	if(in_switch == DemoSwitch::Shapesdemo || DemoSwitch::DisplacementMapping || DemoSwitch::ShadowMap)
	{
		cylinderParts.CreateCylinder(bottom_radius, top_radius, height, slice_count, stack_count, mesh);
	}
	std::vector<Vertex_IA> vertices(mesh.vertices.size());


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




	if (in_switch == DemoSwitch::Shapesdemo || DemoSwitch::DisplacementMapping || DemoSwitch::ShadowMap)
	{
		VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, verticesWithNormals, L"CylinderNormals");
		AddBind(pVertexBuffer);
	}

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, mesh.indices, L"CylinderIndexBuffer");
	AddIndexBuffer(pIndexBuffer);


	switch (currentDemo)
	{

		break;
	case DisplacementMapping:
	case ShadowMap:
	{
		VertexConstantBuffer<cbDefaultVS>* pVCBPerObject =
			new VertexConstantBuffer<cbDefaultVS>(gfx, coneVSCB, 0u, 1u);
		pShadowMapVSDraw = pVCBPerObject->GetVertexConstantBuffer();
		VertexConstantBuffer<ShadowMapGenVS>* pVCBSMGen =
			new VertexConstantBuffer<ShadowMapGenVS>(gfx, shadowMapCbuffer, 0u, 1u);
		pShadomMapGenCB = pVCBSMGen->GetVertexConstantBuffer();
	}
		break;
	default:
		break;
	}





	conePSCB.mat.diffuseAlbedo = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	conePSCB.mat.fresnelR0 = DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f);
	conePSCB.mat.shininess = 0.7f;
	conePSCB.dirLight.strength = DirectX::XMFLOAT3(0.8f, 0.8f, 0.8f);


	if (currentDemo == ShadowMap)
	{
		PixelShaderConstantBuffer<cbDefaultPS>* pLightsCB =
			new PixelShaderConstantBuffer<cbDefaultPS>(gfx, conePSCB, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
		pShadowMapConeDrawPS = pLightsCB->GetPixelShaderConstantBuffer();
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


	

	TextureSampler* pTexSampler = new TextureSampler(gfx, ShaderType::Pixel);
	AddBind(pTexSampler);

}

void Cylinder::DrawCylinder(Graphics& gfx, const DirectX::XMMATRIX& in_world,
	const DirectX::XMMATRIX& in_ViewProj, const DirectX::XMFLOAT3 camPositon)
{
// 	UpdateDisplacementCBuffers(gfx, in_world, in_ViewProj, camPositon);
// 	UpdatePSConstBuffers(gfx, camPositon);
// 	BindAndDrawIndexed(gfx);
}


void Cylinder::UpdateDisplacementCBuffers(Graphics& gfx, const DirectX::XMMATRIX& in_world,
	const DirectX::XMMATRIX& in_ViewProj, const DirectX::XMFLOAT3 in_camera)
{

}

void Cylinder::UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj)
{
}

void Cylinder::UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPositon)
{
}

void Cylinder::IncrementTexArrPos() noexcept
{
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
	DirectX::XMFLOAT3& newLightDirection)
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


	gfx.pgfx_pDeviceContext->PSSetConstantBuffers(0u, 1u, &pShadowMapConeDrawPS);
	gfx.pgfx_pDeviceContext->PSSetShaderResources(2u, 1u, &pShadowMapSRV);
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pShadowMapConeDrawPS, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));

	cbDefaultPS* surface = reinterpret_cast<cbDefaultPS*> (mappedData.pData);
	if (GetAsyncKeyState('1') & 0x8000)
		surface->enableNormalMapping = true;
	else
	{
		surface->enableNormalMapping = false;

	}

	surface->camPositon = newCamPosition;
	surface->lightDirection = newLightDirection;
	gfx.pgfx_pDeviceContext->Unmap(pShadowMapConeDrawPS, 0u);


}



void Cylinder::UpdatePosition(Graphics& gfx, const DirectX::XMMATRIX& in_world)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	gfx.pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &pShadowMapVSDraw);
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pShadowMapVSDraw, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	cbDefaultVS* shadowVS = reinterpret_cast<cbDefaultVS*> (mappedData.pData);
	shadowVS->world = in_world;
	gfx.pgfx_pDeviceContext->Unmap(pShadowMapVSDraw, 0u);
}

DirectionalLight* Cylinder::GetOldLightDirection()
{
	return directionalLight.dirLight;
}

