#include "Skull.h"

CBPerFrame Skull::constBuffPerFrame;

Skull::Skull(Graphics& gfx, const std::wstring& path)
{
	struct Vertices
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
	};
	UINT vertices = 0;
	UINT triangles = 0;

	shadowMaterial.ambient = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	shadowMaterial.diffuse = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f);
	shadowMaterial.specular = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 16.0f);


	directionalLight.mat.ambient = DirectX::XMFLOAT4(0.99f, 0.99f, 0.99f, 1.0f);
	directionalLight.mat.diffuse = DirectX::XMFLOAT4(0.99f, 0.99f, 0.99f, 1.0f);
	directionalLight.mat.specular = DirectX::XMFLOAT4(0.89f, 0.85f, 0.788f, 16.0f);
	directionalLight.dirLight[0].ambient = DirectX::XMFLOAT4(0.015f, 0.015f, 0.015f, 1.0f);
	directionalLight.dirLight[0].diffuse = DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	directionalLight.dirLight[0].direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	directionalLight.dirLight[0].specular = DirectX::XMFLOAT4(0.03f, 0.03f, 0.03f, 1.0f);
	directionalLight.dirLight[1].ambient = DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	directionalLight.dirLight[1].diffuse = DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	directionalLight.dirLight[1].direction = DirectX::XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);
	directionalLight.dirLight[1].specular = DirectX::XMFLOAT4(0.05f, 0.05f, 0.05f, 1.0f);
	directionalLight.dirLight[2].ambient = DirectX::XMFLOAT4(0.0, 0.0f, 0.0f, 1.0f);
	directionalLight.dirLight[2].diffuse = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	directionalLight.dirLight[2].direction = DirectX::XMFLOAT3(0.0f, -0.707f, -0.707f);
	directionalLight.dirLight[2].specular = DirectX::XMFLOAT4(0.02f, 0.02f, 0.02f, 1.0f);



	std::fstream file(path);
	std::string ignore;
	file >> ignore >> vertices;
	file >> ignore >> triangles;
	file >> ignore >> ignore >> ignore >> ignore;

	std::vector<Vertices> verticesFromTXT(vertices);

	for (size_t i = 0; i < vertices; i++)
	{
		file >> verticesFromTXT[i].position.x >> verticesFromTXT[i].position.y >> verticesFromTXT[i].position.z >>
			verticesFromTXT[i].normal.x  >> verticesFromTXT[i].normal.y  >> verticesFromTXT[i].normal.z;
	}
	file >> ignore >> ignore >> ignore;
	UINT indexCount = 3 * triangles;
	std::vector<UINT> indices(indexCount);

	for (size_t i = 0; i < triangles; ++i)
	{
		file >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}
	file.close();


	VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, verticesFromTXT, L"TXT");
	AddBind(pVertexBuffer);

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, indices, L"TXTIndexBuffer");
	AddIndexBuffer(pIndexBuffer);

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(pTopology);

	VertexConstantBuffer<CB_VS_Transform>* pVCBPerObject =
		new VertexConstantBuffer<CB_VS_Transform>(gfx, transformMatrices, 0u, 1u);
	pCopyVCBMatricesSkull = pVCBPerObject->GetVertexConstantBuffer(); //for updating every frame
	AddBind(pVCBPerObject);

	PixelShaderConstantBuffer<CB_PS_DirectionalL_Fog>* pLightsPS =
		new PixelShaderConstantBuffer<CB_PS_DirectionalL_Fog>(gfx, directionalLight, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	AddBind(pLightsPS);

	PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>* pLightsCB =
		new PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>(gfx, pscBuffer, 1u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyPCBLightsSkull = pLightsCB->GetPixelShaderConstantBuffer();
	AddBind(pLightsCB);

}

DirectX::XMMATRIX Skull::GetTransform() const noexcept
{
	return m_Matrix * m_Centered;
}

void Skull::Update(float dt) noexcept
{
	alpha = dt;
}

void Skull::UpdateVertexConstantBuffer(Graphics& gfx)
{

}

void Skull::UpdateLightDirection(Graphics& gfx)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	//discarding and updating everything so we can avoid bug with light on the skull flashing.
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPCBLightsSkull, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CBPerFrame* frame = reinterpret_cast<CBPerFrame*> (mappedData.pData);
	frame->dirLight[0].direction = GetLight(0).direction;
	frame->dirLight[0].ambient = constBuffPerFrame.dirLight[0].ambient;
	frame->dirLight[0].diffuse = constBuffPerFrame.dirLight[0].diffuse;
	frame->dirLight[0].specular = constBuffPerFrame.dirLight[0].specular;

	frame->dirLight[1].direction = GetLight(1).direction;
	frame->dirLight[1].ambient = constBuffPerFrame.dirLight[1].ambient;
	frame->dirLight[1].diffuse = constBuffPerFrame.dirLight[1].diffuse;
	frame->dirLight[1].specular = constBuffPerFrame.dirLight[1].specular;

	frame->dirLight[2].direction = GetLight(2).direction;
	frame->dirLight[2].ambient = constBuffPerFrame.dirLight[2].ambient;
	frame->dirLight[2].diffuse = constBuffPerFrame.dirLight[2].diffuse;
	frame->dirLight[2].specular = constBuffPerFrame.dirLight[2].specular;

	frame->objectMaterial = constBuffPerFrame.objectMaterial;
	frame->cbEyePosition = eyePosition;
	//because of discarding date, we have to keep updating the old light number.
	if (GetAsyncKeyState('0') & 0x8000)
	{
		frame->numLights = 0;
		currentLightNum = 0;
	}
	if (GetAsyncKeyState('1') & 0x8000)
	{
		frame->numLights = 1;
		currentLightNum = 1;
	}

	if (GetAsyncKeyState('2') & 0x8000)
	{
		frame->numLights = 2;
		currentLightNum = 2;
	}

	if (GetAsyncKeyState('3') & 0x8000)
	{
		frame->numLights = 3;
		currentLightNum = 3;
	}
	else
	{
		frame->numLights = currentLightNum;
	}

	gfx.pgfx_pDeviceContext->Unmap(pCopyPCBLightsSkull, 0u);

}

void Skull::SetCameraMatrix(DirectX::XMMATRIX in_matrix) noexcept
{
	m_Matrix = in_matrix;
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

void Skull::UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPositon)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPCBLightsSkull, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
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

	gfx.pgfx_pDeviceContext->Unmap(pCopyPCBLightsSkull, 0u);
}

DirectX::XMMATRIX Skull::GetMirroredSkullTranslation() const
{
	return mirroredSkull;
}

void Skull::SetNewLightDirection(DirectX::XMFLOAT3& lightDirection, UINT index) noexcept
{
	constBuffPerFrame.dirLight[index].direction = lightDirection;
}

void Skull::SetNewLightDirection_(DirectX::XMFLOAT3 lightDirection[3], UINT index) noexcept
{
	constBuffPerFrame.dirLight[index].direction = lightDirection[index];
}

void Skull::UpdateEyePosition(DirectX::XMFLOAT3 eyePos) noexcept
{
	eyePosition = eyePos;
}

void Skull::UpdateMaterial(Graphics& gfx, bool shadow) noexcept
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPCBLightsSkull, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CBPerFrame* frame = reinterpret_cast<CBPerFrame*> (mappedData.pData);
	if (shadow)
	{
		frame->objectMaterial = shadowMaterial;
	}
	else {
		frame->objectMaterial = constBuffPerFrame.objectMaterial;
	}
	/*frame->dirLight[0].direction = GetLight(0).direction;
	frame->dirLight[0].ambient = constBuffPerFrame.dirLight[0].ambient;
	frame->dirLight[0].diffuse = constBuffPerFrame.dirLight[0].diffuse;
	frame->dirLight[0].specular = constBuffPerFrame.dirLight[0].specular;

	frame->dirLight[1].direction = GetLight(1).direction;
	frame->dirLight[1].ambient = constBuffPerFrame.dirLight[1].ambient;
	frame->dirLight[1].diffuse = constBuffPerFrame.dirLight[1].diffuse;
	frame->dirLight[1].specular = constBuffPerFrame.dirLight[1].specular;

	frame->dirLight[2].direction = GetLight(2).direction;
	frame->dirLight[2].ambient = constBuffPerFrame.dirLight[2].ambient;
	frame->dirLight[2].diffuse = constBuffPerFrame.dirLight[2].diffuse;
	frame->dirLight[2].specular = constBuffPerFrame.dirLight[2].specular;

	frame->objectMaterial = constBuffPerFrame.objectMaterial;
	frame->cbEyePosition = eyePosition;
	//because of discarding date, we have to keep updating the old light number.
	if (GetAsyncKeyState('0') & 0x8000)
	{
		frame->numLights = 0;
		currentLightNum = 0;
	}
	if (GetAsyncKeyState('1') & 0x8000)
	{
		frame->numLights = 1;
		currentLightNum = 1;
	}

	if (GetAsyncKeyState('2') & 0x8000)
	{
		frame->numLights = 2;
		currentLightNum = 2;
	}

	if (GetAsyncKeyState('3') & 0x8000)
	{
		frame->numLights = 3;
		currentLightNum = 3;
	}
	else
	{
		frame->numLights = currentLightNum;
	}*/

	gfx.pgfx_pDeviceContext->Unmap(pCopyPCBLightsSkull, 0u);

}

DirectionalLight Skull::GetLight(UINT index) const noexcept
{
	return constBuffPerFrame.dirLight[index];
}
