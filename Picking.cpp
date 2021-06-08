#include "Picking.h"

Picking::Picking(Graphics& gfx)
{
	struct Vertices
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
	};
	UINT vertices = 0;
	UINT triangles = 0;


	std::fstream file(L"models\\car.txt");
	std::string ignore;
	file >> ignore >> vertices;
	file >> ignore >> triangles;
	file >> ignore >> ignore >> ignore >> ignore;

	std::vector<Vertices> verticesFromTXT(vertices);

	for (size_t i = 0; i < vertices; i++)
	{
		file >> verticesFromTXT[i].position.x >> verticesFromTXT[i].position.y >> verticesFromTXT[i].position.z >>
			verticesFromTXT[i].normal.x >> verticesFromTXT[i].normal.y >> verticesFromTXT[i].normal.z;
	}
	file >> ignore >> ignore >> ignore;
	UINT indexCount = 3 * triangles;
	std::vector<UINT> indices(indexCount);

	for (size_t i = 0; i < triangles; ++i)
	{
		file >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}
	file.close();
	
	carPSBuffer.mat.ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	carPSBuffer.mat.diffuse = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	carPSBuffer.mat.specular = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 32.0f);

	carPSBuffer.dirLight[0].ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	carPSBuffer.dirLight[0].diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	carPSBuffer.dirLight[0].direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	carPSBuffer.dirLight[0].specular = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	carPSBuffer.dirLight[1].ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	carPSBuffer.dirLight[1].diffuse = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	carPSBuffer.dirLight[1].direction = DirectX::XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);
	carPSBuffer.dirLight[1].specular = DirectX::XMFLOAT4(0.55f, 0.55f, 0.55f, 1.0f);
	carPSBuffer.dirLight[2].ambient = DirectX::XMFLOAT4(0.5, 0.5f, 0.5f, 1.0f);
	carPSBuffer.dirLight[2].diffuse = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	carPSBuffer.dirLight[2].direction = DirectX::XMFLOAT3(0.0f, -0.707f, -0.707f);
	carPSBuffer.dirLight[2].specular = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);



	VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, verticesFromTXT, L"TXT");
	AddBind(pVertexBuffer);

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, indices, L"TXTIndexBuffer");
	AddIndexBuffer(pIndexBuffer);

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(pTopology);

	VertexConstantBuffer<CB_VS_Transform>* pVCBPerObject =
		new VertexConstantBuffer<CB_VS_Transform>(gfx, transformMatrices, 0u, 1u);
	pCopyVCBMatricesCar = pVCBPerObject->GetVertexConstantBuffer(); 
	AddBind(pVCBPerObject);

	PixelShaderConstantBuffer<CB_PS_DirectionalL_Fog>* pCarM =
		new PixelShaderConstantBuffer<CB_PS_DirectionalL_Fog>(gfx, carPSBuffer, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyCarMaterial = pCarM->GetPixelShaderConstantBuffer();
	AddBind(pCarM);

	PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>* pLightsCB =
		new PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>(gfx, pscBuffer, 1u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyPCBLightsPerFrame = pLightsCB->GetPixelShaderConstantBuffer();
	AddBind(pLightsCB);

}

void Picking::UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBMatricesCar, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CB_VS_Transform* pMatrices = reinterpret_cast<CB_VS_Transform*>(mappedData.pData);
	pMatrices->world = in_world;
	pMatrices->worldInvTranspose = MathHelper::InverseTranspose(in_world);
	pMatrices->worldViewProjection = DirectX::XMMatrixTranspose(in_world * in_ViewProj);
	pMatrices->texTransform = DirectX::XMMatrixIdentity();
	gfx.pgfx_pDeviceContext->Unmap(pCopyVCBMatricesCar, 0u);
}

void Picking::UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPosition)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPCBLightsPerFrame, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
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

	gfx.pgfx_pDeviceContext->Unmap(pCopyPCBLightsPerFrame, 0u);
}
