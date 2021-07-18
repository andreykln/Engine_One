#include "Cylinder.h"
Cylinder::Cylinder(Graphics& gfx,
	float bottom_radius, float top_radius, float height, UINT slice_count, UINT stack_count, DemoSwitch in_switch)
	: currentDemo{in_switch}
{
	if (in_switch == DemoSwitch::LightningCone)
	{
		cylinderParts.CreateCylinderNoCaps(bottom_radius, top_radius, height, slice_count, stack_count, mesh);
	}
	if(in_switch == DemoSwitch::Shapesdemo)
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

	if (in_switch == DemoSwitch::Shapesdemo)
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

	if (in_switch == DemoSwitch::Shapesdemo)
	{
		VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, verticesWithNormals, L"CylinderNormals");
		AddBind(pVertexBuffer);
	}

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, mesh.indices, L"CylinderIndexBuffer");
	AddIndexBuffer(pIndexBuffer);

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(pTopology);

	VertexConstantBuffer<CB_VS_Transform>* pVCBPerObject =
		new VertexConstantBuffer<CB_VS_Transform>(gfx, transformMatrices, 0u, 1u);
	pCopyVCBMatricesCylinder = pVCBPerObject->GetVertexConstantBuffer();
	AddBind(pVCBPerObject);

	PixelShaderConstantBuffer<CB_PS_DirectionalL_Fog>* pLightsPS =
		new PixelShaderConstantBuffer<CB_PS_DirectionalL_Fog>(gfx, directionalLight, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	AddBind(pLightsPS);

	PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>* pLightsCB =
		new PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>(gfx, pscBuffer, 1u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyPCBLightsCylinder = pLightsCB->GetPixelShaderConstantBuffer();
	AddBind(pLightsCB);

	if (currentDemo == DemoSwitch::LightningCone)
	{
		std::wstring LightningArray[60];
		for (UINT i = 0; i < 60; ++i)
		{
			LightningArray[i] = L"Textures\\Lightning\\Spark" + std::to_wstring(i + 1) + L".dds";
		}
		ShaderResourceView* pSRV = new ShaderResourceView(gfx, LightningArray, (UINT)std::size(LightningArray), 1, true);
		AddBind(pSRV);
	}
	if(currentDemo == DemoSwitch::Shapesdemo)
	{
		std::wstring directory[1];
		std::wstring normalMap[1];
		directory[0] = L"Textures\\bricks.dds";
		normalMap[0] = L"Textures\\bricks_nmap.dds";
		ShaderResourceView* pSRV = new ShaderResourceView(gfx, directory, 0u,  (UINT)std::size(directory), ShaderType::Pixel);
		AddBind(pSRV);

		ShaderResourceView* pSRVN = new ShaderResourceView(gfx, normalMap, 1u, 1u, ShaderType::Pixel);
		AddBind(pSRVN);
	}
	TextureSampler* pTexSampler = new TextureSampler(gfx, ShaderType::Pixel);
	AddBind(pTexSampler);


}

void Cylinder::UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBMatricesCylinder, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CB_VS_Transform* pMatrices = reinterpret_cast<CB_VS_Transform*>(mappedData.pData);
	pMatrices->world = in_world;
	pMatrices->worldInvTranspose = MathHelper::InverseTranspose(in_world);
	pMatrices->worldViewProjection = DirectX::XMMatrixTranspose(in_world * in_ViewProj);
	pMatrices->texTransform = DirectX::XMMatrixIdentity();
	gfx.pgfx_pDeviceContext->Unmap(pCopyVCBMatricesCylinder, 0u);
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

