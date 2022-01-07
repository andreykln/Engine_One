#include "Skull.h"

Skull::Skull(Graphics& gfx, const std::wstring& path)
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

	skullMatData.ambient = DirectX::XMFLOAT4(0.66f, 0.662f, 0.663f, 1.0f);
	skullMatData.diffuse = DirectX::XMFLOAT4(0.66f, 0.662f, 0.663f, 1.0f);
	skullMatData.specular = DirectX::XMFLOAT4(0.66f, 0.662f, 0.663f, 16.0f);

	dirLightEX.dirLight.direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	dirLightEX.dirLight.strength = DirectX::XMFLOAT3(0.8f, 0.8f, 0.8f);
	//data for bone skull
// 	dirLightEX.mat.diffuseAlbedo = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
// 	dirLightEX.mat.fresnelR0 = DirectX::XMFLOAT3(0.08f, 0.08f, 0.08f);
// 	dirLightEX.mat.shininess = 0.7f; //the greater the number the more spread the specular highlight
// 	dirLightEX.ambientLight = DirectX::XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);

	//data for mirror skull
	dirLightEX.mat.diffuseAlbedo = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	dirLightEX.mat.fresnelR0 = DirectX::XMFLOAT3(0.6f, 0.6f, 0.6f);
	dirLightEX.mat.shininess = 0.8f;
	dirLightEX.dirLight.strength = DirectX::XMFLOAT3(0.8f, 0.8f, 0.8f);



	std::fstream file(path);
	std::string ignore;
	file >> ignore >> vertices;
	file >> ignore >> triangles;
	file >> ignore >> ignore >> ignore >> ignore;

	std::vector<vbPosNormalTexTangent> verticesFromTXT(vertices);

	for (size_t i = 0; i < vertices; i++)
	{
		using namespace DirectX;
		file >> verticesFromTXT[i].pos.x >> verticesFromTXT[i].pos.y >> verticesFromTXT[i].pos.z >>
			verticesFromTXT[i].normal.x >> verticesFromTXT[i].normal.y >> verticesFromTXT[i].normal.z;
		verticesFromTXT[i].tex = { 0.0f, 0.0f };

		XMVECTOR P = XMLoadFloat3(&verticesFromTXT[i].pos);
		XMVECTOR N = XMLoadFloat3(&verticesFromTXT[i].normal);
		// Generate a tangent vector so normal mapping works.  We aren't applying
		// a texture map to the skull, so we just need any tangent vector so that
		// the math works out to give us the original interpolated vertex normal.
		XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		auto a = XMVector3Dot(N, up);
		auto b = XMVectorGetX(XMVector3Dot(N, up));
		auto c = fabsf(XMVectorGetX(XMVector3Dot(N, up)));

		if (fabsf(XMVectorGetX(XMVector3Dot(N, up))) < 1.0f - 0.001f)
		{
			XMVECTOR T = XMVector3Normalize(XMVector3Cross(up, N));
			DirectX::XMStoreFloat3(&verticesFromTXT[i].tangent, T);
		}
		else
		{
			up = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
			XMVECTOR T = XMVector3Normalize(XMVector3Cross(N, up));
			DirectX::XMStoreFloat3(&verticesFromTXT[i].tangent, T);
		}

	}
	file >> ignore >> ignore >> ignore;


	UINT indexCount = 3 * triangles;
	std::vector<UINT> indices(indexCount);

	for (size_t i = 0; i < triangles; ++i)
	{
		file >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}
	file.close();

	stride = sizeof(vbPosNormalTexTangent);
	pVertexBuffer = gfx.CreateVertexBuffer(verticesFromTXT, false, false, L"SkullVerticess");

// 	VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, verticesFromTXT, L"TXT");
// 	AddBind(pVertexBuffer);

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, indices, L"TXTIndexBuffer");
	AddIndexBuffer(pIndexBuffer);

	VertexConstantBuffer<CB_VS_ShadowMapDrawWithSSAO>* pVCBPerObject =
		new VertexConstantBuffer<CB_VS_ShadowMapDrawWithSSAO>(gfx, shadowMapVSDraw, 0u, 1u);
	pShadowMapVSDraw = pVCBPerObject->GetVertexConstantBuffer();

	////// used in new normal map
	VertexConstantBuffer<cbCreateNormalMap>* pVCBNMap =
		new VertexConstantBuffer<cbCreateNormalMap>(gfx, normalMapData, 0u, 1u);
	pNormalMapGenerate = pVCBNMap->GetVertexConstantBuffer();
	////


	VertexConstantBuffer<ShadowMapGenVS>* pVCBSMGen =
		new VertexConstantBuffer<ShadowMapGenVS>(gfx, shadowMapCbuffer, 0u, 1u);
	pShadomMapGenCB = pVCBSMGen->GetVertexConstantBuffer();

	PixelShaderConstantBuffer<CB_PS_DirectionalEX_Fog>* pLightsPS =
		new PixelShaderConstantBuffer<CB_PS_DirectionalEX_Fog>(gfx, dirLightEX, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pLightDirectionPSCbuffer = pLightsPS->GetPixelShaderConstantBuffer();

	PixelShaderConstantBuffer<CB_PS_ShadowMapDraw>* pLightsCB =
		new PixelShaderConstantBuffer<CB_PS_ShadowMapDraw>(gfx, shadowMapDraw, 1u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyPCBLightsCylinder = pLightsCB->GetPixelShaderConstantBuffer();
}


UINT* Skull::GetStride()
{
	return &stride;
}

UINT* Skull::GetOffset()
{
	return &offset;
}

ID3D11Buffer**  Skull::GetVertexBuffer()
{
	return &pVertexBuffer;
}

void Skull::UpdateShadomMapGenBuffers(Graphics& gfx, const DirectX::XMMATRIX& in_lightWorld, DirectX::XMFLOAT3 newCamPosition)
{
	gfx.pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &pShadomMapGenCB);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pShadomMapGenCB, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	ShadowMapGenVS* pMatrices = reinterpret_cast<ShadowMapGenVS*>(mappedData.pData);
	pMatrices->lightWVP = DirectX::XMMatrixTranspose(in_lightWorld);
	pMatrices->texTransform = DirectX::XMMatrixIdentity();
	gfx.pgfx_pDeviceContext->Unmap(pShadomMapGenCB, 0u);
}

void Skull::UpdateShadowMapDrawBuffers(Graphics& gfx, DirectX::XMFLOAT3 newCamPosition,
	const DirectX::XMMATRIX& newShadowTransform, const DirectX::XMMATRIX& in_world,
	const DirectX::XMMATRIX& in_ViewProj, ID3D11ShaderResourceView* pShadowMapSRV, DirectX::XMFLOAT3& newLightDirection)
{
	DirectX::XMMATRIX toTexSpace(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);
	DirectX::XMMATRIX worldViewProjection = in_world * in_ViewProj;

	D3D11_MAPPED_SUBRESOURCE mappedData;
	gfx.pgfx_pDeviceContext->VSSetConstantBuffers(0u, 1u, &pShadowMapVSDraw);
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pShadowMapVSDraw, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CB_VS_ShadowMapDrawWithSSAO* shadowVS = reinterpret_cast<CB_VS_ShadowMapDrawWithSSAO*> (mappedData.pData);
	shadowVS->texTransform = DirectX::XMMatrixIdentity();
	shadowVS->shadowTransform = newShadowTransform;
	shadowVS->world = in_world;
	shadowVS->worldInvTranspose = MathHelper::InverseTranspose(in_world);
	shadowVS->worldViewProjection = DirectX::XMMatrixTranspose(worldViewProjection);
	shadowVS->viewProjTex = DirectX::XMMatrixTranspose(worldViewProjection * toTexSpace);
	gfx.pgfx_pDeviceContext->Unmap(pShadowMapVSDraw, 0u);

	gfx.pgfx_pDeviceContext->PSSetConstantBuffers(1u, 1u, &pCopyPCBLightsCylinder);
	gfx.pgfx_pDeviceContext->PSSetShaderResources(2u, 1u, &pShadowMapSRV);
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPCBLightsCylinder, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CB_PS_ShadowMapDraw* frame = reinterpret_cast<CB_PS_ShadowMapDraw*> (mappedData.pData);
	if (GetAsyncKeyState('5') & 0x8000)
	{
		frame->useSSAO = false;

	}
	else
	{
		frame->useSSAO = true;
	}
	frame->cameraPositon = newCamPosition;
	frame->lightDirection = newLightDirection;
	gfx.pgfx_pDeviceContext->Unmap(pCopyPCBLightsCylinder, 0u);

	gfx.pgfx_pDeviceContext->PSSetConstantBuffers(0u, 1u, &pLightDirectionPSCbuffer);
}

void Skull::UpdateNormalMap(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewM,
	const DirectX::XMMATRIX& in_ViewProjection)
{

// 	D3D11_MAPPED_SUBRESOURCE mappedData;
// 	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pNormalMapGenerate, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
// 	cbCreateNormalMap* cBuffer = reinterpret_cast<cbCreateNormalMap*> (mappedData.pData);
// 	cBuffer->worldInvTransposeView = (MathHelper::InverseTranspose(skullWorld) * DirectX::XMMatrixTranspose(in_ViewM));
// 	cBuffer->worldView = DirectX::XMMatrixTranspose(skullWorld * in_ViewM);
// 	cBuffer->worldViewProjection = DirectX::XMMatrixTranspose(skullWorld * in_ViewProjection);
// 	gfx.pgfx_pDeviceContext->Unmap(pNormalMapGenerate, 0u);
}
