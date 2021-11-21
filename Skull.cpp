#include "Skull.h"

// CBPerFrame Skull::constBuffPerFrame;

Skull::Skull(Graphics& gfx, const std::wstring& path, DemoSwitch in_currentDemo)
	: currentDemo(in_currentDemo)
{
	struct Vertices
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
	};
	struct VerticesSM
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT4 color;
	};

	UINT vertices = 0;
	UINT triangles = 0;



	directionalLight.mat.ambient =  DirectX::XMFLOAT4(0.66f, 0.662f, 0.663f, 1.0f);
	directionalLight.mat.diffuse =  DirectX::XMFLOAT4(0.66f, 0.662f, 0.663f, 1.0f);
	directionalLight.mat.specular = DirectX::XMFLOAT4(0.86f, 0.862f, 0.863f, 64.0f);


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


	skullMatData.ambient = DirectX::XMFLOAT4(0.66f, 0.662f, 0.663f, 1.0f);
	skullMatData.diffuse = DirectX::XMFLOAT4(0.66f, 0.662, 0.663f, 1.0f);
	skullMatData.specular = DirectX::XMFLOAT4(0.66f, 0.662f, 0.663f, 16.0f);

	dirLightEX.dirLight.direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	dirLightEX.dirLight.strength = DirectX::XMFLOAT3(0.9f, 0.8f, 0.7f);
	dirLightEX.mat.diffuseAlbedo = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	dirLightEX.mat.fresnelR0 = DirectX::XMFLOAT3(0.08f, 0.08f, 0.08f);
	dirLightEX.mat.roughness = 0.3f; //the greater the number the more spread the specular highlight
	dirLightEX.ambientLight = DirectX::XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);

	std::fstream file(path);
	std::string ignore;
	file >> ignore >> vertices;
	file >> ignore >> triangles;
	file >> ignore >> ignore >> ignore >> ignore;

	std::vector<Vertices> verticesFromTXT(vertices);
// 	std::vector<VerticesSM> verticesFromTXTSM(vertices);

// 	if (currentDemo == ShadowMap)
// 	{
// 		for (size_t i = 0; i < vertices; i++)
// 		{
// 			file >> verticesFromTXTSM[i].position.x >> verticesFromTXTSM[i].position.y >> verticesFromTXTSM[i].position.z >>
// 				verticesFromTXTSM[i].normal.x >> verticesFromTXTSM[i].normal.y >> verticesFromTXTSM[i].normal.z;
// 			verticesFromTXTSM[i].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
// 		}
// 		file >> ignore >> ignore >> ignore;
// 	}
// 	else
// 	{
		for (size_t i = 0; i < vertices; i++)
		{
			file >> verticesFromTXT[i].position.x >> verticesFromTXT[i].position.y >> verticesFromTXT[i].position.z >>
				verticesFromTXT[i].normal.x >> verticesFromTXT[i].normal.y >> verticesFromTXT[i].normal.z;
		}
		file >> ignore >> ignore >> ignore;
// 	}


	UINT indexCount = 3 * triangles;
	std::vector<UINT> indices(indexCount);

	for (size_t i = 0; i < triangles; ++i)
	{
		file >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}
	file.close();

// 	if (currentDemo == ShadowMap)
// 	{
// 		VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, verticesFromTXTSM, L"TXT");
// 		AddBind(pVertexBuffer);
// 	}
// 	else
// 	{
		VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, verticesFromTXT, L"TXT");
		AddBind(pVertexBuffer);
// 	}

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, indices, L"TXTIndexBuffer");
	AddIndexBuffer(pIndexBuffer);


	if (currentDemo == ShadowMap)
	{
		VertexConstantBuffer<CB_VS_ShadowMapDraw>* pVCBPerObject =
			new VertexConstantBuffer<CB_VS_ShadowMapDraw>(gfx, shadowMapVSDraw, 0u, 1u);
		pShadowMapVSDraw = pVCBPerObject->GetVertexConstantBuffer();
		VertexConstantBuffer<ShadowMapGenVS>* pVCBSMGen =
			new VertexConstantBuffer<ShadowMapGenVS>(gfx, shadowMapCbuffer, 0u, 1u);
		pShadomMapGenCB = pVCBSMGen->GetVertexConstantBuffer();
	}

	else
	{
		VertexConstantBuffer<CB_VS_Transform>* pVCBPerObject =
			new VertexConstantBuffer<CB_VS_Transform>(gfx, transformMatrices, 0u, 1u);
		pCopyVCBMatricesSkull = pVCBPerObject->GetVertexConstantBuffer(); //for updating every frame
		AddBind(pVCBPerObject);
	}


	if (currentDemo == ShadowMap)
	{
		PixelShaderConstantBuffer<CB_PS_DirectionalEX_Fog>* pLightsPS =
			new PixelShaderConstantBuffer<CB_PS_DirectionalEX_Fog>(gfx, dirLightEX, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
		pLightDirectionPSCbuffer = pLightsPS->GetPixelShaderConstantBuffer();

		PixelShaderConstantBuffer<CB_PS_ShadowMapDraw>* pLightsCB =
			new PixelShaderConstantBuffer<CB_PS_ShadowMapDraw>(gfx, shadowMapDraw, 1u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
		pCopyPCBLightsCylinder = pLightsCB->GetPixelShaderConstantBuffer();
	}


	if (currentDemo == Shapesdemo)
	{
		PixelShaderConstantBuffer<CB_PS_DirectionalL_Fog>* pLightsPS =
			new PixelShaderConstantBuffer<CB_PS_DirectionalL_Fog>(gfx, directionalLight, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
		AddBind(pLightsPS);

		PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>* pLightsCB =
			new PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>(gfx, pscBuffer, 1u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
		pCopyPCBLightsSkull = pLightsCB->GetPixelShaderConstantBuffer();
		AddBind(pLightsCB);
	}


}

void Skull::DrawSkull(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj, DirectX::XMFLOAT3 camPosition)
{
	UpdateVSMatrices(gfx, in_world,	in_ViewProj);
	UpdatePSConstBuffers(gfx, camPosition);
	this->BindAndDrawIndexed(gfx);
}

void Skull::UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBMatricesSkull, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CB_VS_Transform* pMatrices = reinterpret_cast<CB_VS_Transform*>(mappedData.pData);
	pMatrices->world = in_world;
	pMatrices->worldInvTranspose = MathHelper::InverseTranspose(in_world);
	pMatrices->worldViewProjection = DirectX::XMMatrixTranspose(in_world * in_ViewProj);
	pMatrices->texTransform = DirectX::XMMatrixIdentity();
	gfx.pgfx_pDeviceContext->Unmap(pCopyVCBMatricesSkull, 0u);
}

void Skull::UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPosition)
{
	if (pCopyPCBLightsSkull)
	{
		D3D11_MAPPED_SUBRESOURCE mappedData;
		DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPCBLightsSkull, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
		CB_PS_PerFrameUpdate* frame = reinterpret_cast<CB_PS_PerFrameUpdate*> (mappedData.pData);
		frame->cameraPositon = camPosition;

		if (GetAsyncKeyState('0') & 0x8000)
			frame->numberOfLights = 0;
		if (GetAsyncKeyState('1') & 0x8000)
			frame->numberOfLights = 1;
		if (GetAsyncKeyState('2') & 0x8000)
			frame->numberOfLights = 2;
		if (GetAsyncKeyState('3') & 0x8000)
			frame->numberOfLights = 3;

		gfx.pgfx_pDeviceContext->Unmap(pCopyPCBLightsSkull, 0u);
	}


}




void Skull::UpdateShadomMapGenBuffers(Graphics& gfx, const DirectX::XMMATRIX& in_lightWorld, DirectX::XMFLOAT3 newCamPosition)
{
	gfx.pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &pShadomMapGenCB);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pShadomMapGenCB, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	ShadowMapGenVS* pMatrices = reinterpret_cast<ShadowMapGenVS*>(mappedData.pData);
	pMatrices->lightWVP = DirectX::XMMatrixTranspose(in_lightWorld);
	pMatrices->texTransform = DirectX::XMMatrixIdentity();
	gfx.pgfx_pDeviceContext->Unmap(pShadomMapGenCB, 0u);
}

void Skull::UpdateShadowMapDrawBuffers(Graphics& gfx, DirectX::XMFLOAT3 newCamPosition, const DirectX::XMMATRIX& newShadowTransform, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj, ID3D11ShaderResourceView* pShadowMapSRV, DirectX::XMFLOAT3* newLightDirection)
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
	frame->lightDirection = newLightDirection[0];
	gfx.pgfx_pDeviceContext->Unmap(pCopyPCBLightsCylinder, 0u);

	gfx.pgfx_pDeviceContext->PSSetConstantBuffers(0u, 1u, &pLightDirectionPSCbuffer);
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pLightDirectionPSCbuffer, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CB_PS_DirectionalEX_Fog* test = reinterpret_cast<CB_PS_DirectionalEX_Fog*> (mappedData.pData);

	if (GetAsyncKeyState('0') & 0x8000)
		test->mat.roughness = 0.9f;

	else
	{
		test->mat.roughness = 0.3f;

	}
	gfx.pgfx_pDeviceContext->Unmap(pLightDirectionPSCbuffer, 0u);

}
