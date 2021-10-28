#include "Hills.h"
#include <cmath>

Hills::Hills(Graphics& gfx, float in_width, float in_depth, UINT in_m, UINT in_n, DemoSwitch demo)
	: width(in_width), depth(in_depth), m(in_m), n(in_n), currentDemo(demo)
{
	landscapeGenerated.CreateGrid(width, depth, m, n, grid);
	std::vector<Vertices_Full> vertices(grid.vertices.size());

	switch (currentDemo)
	{
	case Shapesdemo:
	case ShadowMap:
	{
		directionalLight.dirLight[0].ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		directionalLight.dirLight[0].diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		directionalLight.dirLight[0].direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
		directionalLight.dirLight[0].specular = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

		directionalLight.dirLight[1].ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
		directionalLight.dirLight[1].diffuse = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
		directionalLight.dirLight[1].direction = DirectX::XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);
		directionalLight.dirLight[1].specular = DirectX::XMFLOAT4(0.35f, 0.35f, 0.35f, 1.0f);

		directionalLight.dirLight[2].ambient = DirectX::XMFLOAT4(0.5, 0.5f, 0.5f, 1.0f);
		directionalLight.dirLight[2].diffuse = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
		directionalLight.dirLight[2].direction = DirectX::XMFLOAT3(0.0f, -0.707f, -0.707f);
		directionalLight.dirLight[2].specular = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

		directionalLight.mat.ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		directionalLight.mat.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		directionalLight.mat.specular = DirectX::XMFLOAT4(0.15f, 0.15f, 0.15f, 8.0f);
		directionalLight.mat.reflect = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

		for (size_t i = 0; i < grid.vertices.size(); ++i)
		{
			DirectX::XMFLOAT3 p = grid.vertices[i].position;
			vertices[i].pos = p;
			vertices[i].normal = DirectX::XMFLOAT3{ 0.0f, 1.0f, 0.0f };
			vertices[i].tex = grid.vertices[i].TexC;
			vertices[i].tangent = grid.vertices[i].tangentU;
		}
	}
		break;
	case HillsDemo:
	{
		directionalLight.dirLight[0].ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		directionalLight.dirLight[0].diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		directionalLight.dirLight[0].direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
		directionalLight.dirLight[0].specular = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

		directionalLight.dirLight[1].ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
		directionalLight.dirLight[1].diffuse = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
		directionalLight.dirLight[1].direction = DirectX::XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);
		directionalLight.dirLight[1].specular = DirectX::XMFLOAT4(0.35f, 0.35f, 0.35f, 1.0f);

		directionalLight.dirLight[2].ambient = DirectX::XMFLOAT4(0.5, 0.5f, 0.5f, 1.0f);
		directionalLight.dirLight[2].diffuse = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
		directionalLight.dirLight[2].direction = DirectX::XMFLOAT3(0.0f, -0.707f, -0.707f);
		directionalLight.dirLight[2].specular = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

		directionalLight.mat.ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		directionalLight.mat.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		directionalLight.mat.specular = DirectX::XMFLOAT4(0.15f, 0.15f, 0.15f, 8.0f);
		directionalLight.mat.reflect = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	}
	break;
	case HillsAllLight:
	{
		allLight.dirLight[0].ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		allLight.dirLight[0].diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		allLight.dirLight[0].direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
		allLight.dirLight[0].specular = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

		allLight.dirLight[1].ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
		allLight.dirLight[1].diffuse = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
		allLight.dirLight[1].direction = DirectX::XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);
		allLight.dirLight[1].specular = DirectX::XMFLOAT4(0.35f, 0.35f, 0.35f, 1.0f);

		allLight.dirLight[2].ambient = DirectX::XMFLOAT4(0.5, 0.5f, 0.5f, 1.0f);
		allLight.dirLight[2].diffuse = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
		allLight.dirLight[2].direction = DirectX::XMFLOAT3(0.0f, -0.707f, -0.707f);
		allLight.dirLight[2].specular = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

		allLight.mat.ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		allLight.mat.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		allLight.mat.specular = DirectX::XMFLOAT4(0.15f, 0.15f, 0.15f, 8.0f);
		allLight.mat.reflect = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

		allLight.pointLight.ambient = DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
		allLight.pointLight.diffuse = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
		allLight.pointLight.specular = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
		allLight.pointLight.attenuation = DirectX::XMFLOAT3(0.0f, 0.1f, 0.0f);
		allLight.pointLight.range = 25.0f;

		allLight.spotLight.ambient = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		allLight.spotLight.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		allLight.spotLight.specular = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		allLight.spotLight.attenuation = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
		allLight.spotLight.range = 10000.0f;
		allLight.spotLight.spot = 96.0f;


	}
		break;
	default:
		break;
	}

	switch (currentDemo)
	{
	case HillsDemo:
	case HillsAllLight:
	{
		treesPositions.resize(25);
		size_t j = 0; //trees index
		for (size_t i = 0; i < grid.vertices.size(); ++i)
		{

			DirectX::XMFLOAT3 p = grid.vertices[i].position;
			p.y = GetHeight(p.x, p.z);
			vertices[i].pos = p;
			vertices[i].normal = GetHillNormal(p.x, p.z);
			vertices[i].tex = grid.vertices[i].TexC;

			if (i % 100 == 0)
			{
				treesPositions[j].pos = vertices[i].pos;
				treesPositions[j].pos.y += 10.0f;
				treesPositions[j].size = DirectX::XMFLOAT2{ 20.0f, 10.0f };
				j++;
			}
		}
	}
		break;
	default:
		break;
	}


	VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, vertices, L"Hills");
	AddBind(pVertexBuffer);

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, grid.indices, L"HillsIndexBuffer");
	AddIndexBuffer(pIndexBuffer);


	switch (currentDemo)
	{
	case Shapesdemo:
	case ShadowMap:
	{
		VertexConstantBuffer<CB_VS_TransformWithCameraPosition>* pVSCB = 
			new VertexConstantBuffer<CB_VS_TransformWithCameraPosition>(gfx, transformMatricesWithCameraPos, 0u, 1u);
		pCopyVCBMatricesHills = pVSCB->GetVertexConstantBuffer();
		AddBind(pVSCB);
		break;
	}

	case HillsAllLight:
	case HillsDemo:
	{
		VertexConstantBuffer<CB_VS_Transform>* pVSCB = new VertexConstantBuffer<CB_VS_Transform>(gfx, transformMatrices, 0u, 1u);
		pCopyVCBMatricesHills = pVSCB->GetVertexConstantBuffer();
		AddBind(pVSCB);
	}
	break;
	}



	switch (currentDemo)
	{
	case Shapesdemo:
	case HillsDemo:
	case ShadowMap:
	{
		PixelShaderConstantBuffer<CB_PS_DirectionalL_Fog>* pLightsPS =
			new PixelShaderConstantBuffer<CB_PS_DirectionalL_Fog>(gfx, directionalLight, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
		AddBind(pLightsPS);

		PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>* pLightsCB =
			new PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>(gfx, pscBuffer, 1u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
		pCopyPCBLightsHills = pLightsCB->GetPixelShaderConstantBuffer();
		AddBind(pLightsCB);
	}
	break;
	case HillsAllLight:
	{
		PixelShaderConstantBuffer<CB_PS_Dir_Point_Spot_Fog_Lights>* pAllLightsb =
			new PixelShaderConstantBuffer<CB_PS_Dir_Point_Spot_Fog_Lights>(gfx, allLight, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
		pCopyAllLights = pAllLightsb->GetPixelShaderConstantBuffer();
		AddBind(pAllLightsb);
	}
		break;
	default:
		break;
	}

	switch (currentDemo)
	{

	case Shapesdemo:
	{
		DomainShaderConstantBuffer<CB_CameraPosition_ViewProj>* pDSCamPos =
			new DomainShaderConstantBuffer<CB_CameraPosition_ViewProj>(
				gfx, dsBufferCameraPosition, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
		pCopyDomainShaderBuffer = pDSCamPos->GetDomainShaderConstantBuffer();
		AddBind(pDSCamPos);
		break;
	}
	default:
		break;
	}


	std::wstring directory[1];
	std::wstring normalMap[1];

	switch (currentDemo)
	{
	case Shapesdemo:
	{
		directory[0] = L"Textures\\stones.dds";
		normalMap[0] = L"Textures\\stones_nmap.dds";

		ShaderResourceView* pSRVHeightMap = new ShaderResourceView(
								gfx, normalMap, 1u, (UINT)std::size(normalMap), ShaderType::Domain);
		AddBind(pSRVHeightMap);
		break;
	}
	case HillsDemo:
	case HillsAllLight:
	{
		directory[0] = L"Textures\\grass.dds";
		break;
	}
	case ShadowMap:
	{
		directory[0] = L"Textures\\floor.dds";
		normalMap[0] = L"Textures\\floor_nmap.dds";

		ShaderResourceView* pSRVHeightMap = new ShaderResourceView(
			gfx, normalMap, 1u, (UINT)std::size(normalMap), ShaderType::Pixel);
		AddBind(pSRVHeightMap);
		break;
	}
	default:
		break;
	}


	ShaderResourceView* pSRV = new ShaderResourceView(gfx, directory, 0u, (UINT)std::size(directory), ShaderType::Pixel);
	AddBind(pSRV);
	if (currentDemo == DemoSwitch::Shapesdemo)
	{
		ShaderResourceView* pSRVnMap = new ShaderResourceView(gfx, normalMap, 1u, (UINT)std::size(normalMap), ShaderType::Pixel);
		AddBind(pSRVnMap);
	}


	TextureSampler* pTexSampler = new TextureSampler(gfx, ShaderType::Pixel);
	AddBind(pTexSampler);
	if (currentDemo == DemoSwitch::Shapesdemo)
	{
		TextureSampler* pDomainTexSampler = new TextureSampler(gfx, ShaderType::Domain);
		AddBind(pDomainTexSampler);
	}
}



DirectX::XMFLOAT3 Hills::GetHillNormal(float x, float z) const
{
	DirectX::XMFLOAT3 n(
		-0.03f * z * cosf(0.1f * x) - 0.3f * cosf(0.1f * z),
		1.0f,
		-0.3f * sinf(0.1f * x) + 0.03f * x * sinf(0.1f * z));
	DirectX::XMVECTOR unitNormal = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&n));
	DirectX::XMStoreFloat3(&n, unitNormal);
	return n;
}


void Hills::SetVerticesWidth(UINT in_vertWidth) noexcept
{
	m = in_vertWidth;
}

void Hills::SetVerticesDepth(UINT in_vertDepth) noexcept
{
	n = in_vertDepth;
}



void Hills::UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world,
	const DirectX::XMMATRIX& in_ViewProj, const DirectX::XMFLOAT3 in_camera)
{
	switch (currentDemo)
	{
	case ShadowMap:
	{
		D3D11_MAPPED_SUBRESOURCE mappedData;
		DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBMatricesHills, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
		CB_VS_TransformWithCameraPosition* pMatrices = reinterpret_cast<CB_VS_TransformWithCameraPosition*>(mappedData.pData);
		pMatrices->world = DirectX::XMMatrixTranspose(in_world);
		pMatrices->worldInvTranspose = MathHelper::InverseTranspose(in_world);
		pMatrices->worldViewProjection = DirectX::XMMatrixTranspose(in_world * in_ViewProj);
		pMatrices->texTransform = grassScaling;
		pMatrices->cameraPosition = in_camera;
		gfx.pgfx_pDeviceContext->Unmap(pCopyVCBMatricesHills, 0u);
		break;
	}
	case Shapesdemo:
	{
		D3D11_MAPPED_SUBRESOURCE mappedData;
		DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBMatricesHills, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
		CB_VS_TransformWithCameraPosition* pMatrices = reinterpret_cast<CB_VS_TransformWithCameraPosition*>(mappedData.pData);
		pMatrices->world = DirectX::XMMatrixTranspose(in_world);
		pMatrices->worldInvTranspose = MathHelper::InverseTranspose(in_world);
		pMatrices->worldViewProjection = DirectX::XMMatrixTranspose(in_world * in_ViewProj);
		pMatrices->texTransform = grassScaling;
		pMatrices->cameraPosition = in_camera;
		gfx.pgfx_pDeviceContext->Unmap(pCopyVCBMatricesHills, 0u);

		DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyDomainShaderBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
		CB_CameraPosition_ViewProj* pDSBuffer = reinterpret_cast<CB_CameraPosition_ViewProj*>(mappedData.pData);
		pDSBuffer->cameraPosition = in_camera;
		pDSBuffer->viewProjection = DirectX::XMMatrixTranspose(in_ViewProj);
		gfx.pgfx_pDeviceContext->Unmap(pCopyDomainShaderBuffer, 0u);
		break;
	}

	case HillsAllLight:
	case HillsDemo:
	{
		D3D11_MAPPED_SUBRESOURCE mappedData;
		DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBMatricesHills, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
		CB_VS_Transform* pMatrices = reinterpret_cast<CB_VS_Transform*>(mappedData.pData);
		pMatrices->world = in_world;
		pMatrices->worldInvTranspose = MathHelper::InverseTranspose(in_world);
		pMatrices->worldViewProjection = DirectX::XMMatrixTranspose(in_world * in_ViewProj);
		pMatrices->texTransform = grassScaling;
		gfx.pgfx_pDeviceContext->Unmap(pCopyVCBMatricesHills, 0u);

	}
	break;
	}

}

void Hills::UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPositon)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPCBLightsHills, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
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

	gfx.pgfx_pDeviceContext->Unmap(pCopyPCBLightsHills, 0u);

}

void Hills::UpdatePSAllLights(Graphics& gfx, DirectX::XMFLOAT3 camPosition, DirectX::XMFLOAT3 camDirection, float totalTime)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyAllLights, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CB_PS_Dir_Point_Spot_Fog_Lights* frame = reinterpret_cast<CB_PS_Dir_Point_Spot_Fog_Lights*> (mappedData.pData);
	frame->cameraPosition = camPosition;
	frame->pointLight.position.x = 70.0f * cosf(0.2f * totalTime);
	frame->pointLight.position.z = 70.0f * sinf(0.2f * totalTime);
	frame->pointLight.position.y = MathHelper::Max(GetHeight(frame->pointLight.position.x, frame->pointLight.position.z), -3.0f) + 10.0f;
	frame->spotLight.position = camPosition;
	frame->spotLight.direction = camDirection;


	if (GetAsyncKeyState('0') & 0x8000)
		frame->numberOfLights = 0;
	if (GetAsyncKeyState('1') & 0x8000)
		frame->numberOfLights = 1;
	if (GetAsyncKeyState('2') & 0x8000)
		frame->numberOfLights = 2;
	if (GetAsyncKeyState('3') & 0x8000)
		frame->numberOfLights = 3;

	gfx.pgfx_pDeviceContext->Unmap(pCopyAllLights, 0u);

}

DirectX::XMMATRIX Hills::GetHillsOffset() const
{
	return offsetForHillsWithWaves;
}

float Hills::GetHeight(float x, float z) const
{
	return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z));
}
