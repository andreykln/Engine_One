#include "MirrorRoom.h"

MirrorRoom::MirrorRoom(Graphics& gfx)
{
	std::vector<Vertex_IA> vertices;
// 	Vertex_IA test  (-3.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 4.0f);
	vertices.resize(30u);
	// Floor: Observe we tile texture coordinates.
	vertices[0] = Vertex_IA::Vertex_IA(-3.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 4.0f);
	vertices[1] = Vertex_IA::Vertex_IA(-3.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	vertices[2] = Vertex_IA::Vertex_IA(7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 4.0f, 0.0f);
	vertices[3] = Vertex_IA::Vertex_IA(-3.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 4.0f);
	vertices[4] = Vertex_IA::Vertex_IA(7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 4.0f, 0.0f);
	vertices[5] = Vertex_IA::Vertex_IA(7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 4.0f, 4.0f);

	// Wall: Observe we tile texture coordinates, and that we
	// leave a gap in the middle for the mirror.
	vertices[6] = Vertex_IA::Vertex_IA(-3.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 2.0f);
	vertices[7] = Vertex_IA::Vertex_IA(-3.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	vertices[8] = Vertex_IA::Vertex_IA(-2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.5f, 0.0f);
	vertices[9] = Vertex_IA::Vertex_IA(-3.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 2.0f);

	vertices[10] = Vertex_IA::Vertex_IA(-2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.5f, 0.0f);
	vertices[11] = Vertex_IA::Vertex_IA(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.5f, 2.0f);
	vertices[12] = Vertex_IA::Vertex_IA(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 2.0f);
	vertices[13] = Vertex_IA::Vertex_IA(2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	vertices[14] = Vertex_IA::Vertex_IA(7.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 2.0f, 0.0f);

	vertices[15] = Vertex_IA::Vertex_IA(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 2.0f);
	vertices[16] = Vertex_IA::Vertex_IA(7.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 2.0f, 0.0f);
	vertices[17] = Vertex_IA::Vertex_IA(7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 2.0f, 2.0f);
	vertices[18] = Vertex_IA::Vertex_IA(-3.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);

	vertices[19] = Vertex_IA::Vertex_IA(-3.5f, 6.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	vertices[20] = Vertex_IA::Vertex_IA(7.5f, 6.0f, 0.0f, 0.0f, 0.0f, -1.0f, 6.0f, 0.0f);
	vertices[21] = Vertex_IA::Vertex_IA(-3.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	vertices[22] = Vertex_IA::Vertex_IA(7.5f, 6.0f, 0.0f, 0.0f, 0.0f, -1.0f, 6.0f, 0.0f);
	vertices[23] = Vertex_IA::Vertex_IA(7.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 6.0f, 1.0f);

	// Mirror
	vertices[24] = Vertex_IA::Vertex_IA(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	vertices[25] = Vertex_IA::Vertex_IA(-2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	vertices[26] = Vertex_IA::Vertex_IA(2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	vertices[27] = Vertex_IA::Vertex_IA(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	vertices[28] = Vertex_IA::Vertex_IA(2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	vertices[29] = Vertex_IA::Vertex_IA(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	


	floorMaterial.ambient = DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	floorMaterial.diffuse = DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	floorMaterial.specular = DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 16.0f);

	wallMaterial.ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	wallMaterial.diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	wallMaterial.specular = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 16.0f);

	mirrorMaterial.ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	mirrorMaterial.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.2f);
	mirrorMaterial.specular = DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 16.0f);


	pscBuffer.dirLight[0].ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	pscBuffer.dirLight[0].diffuse = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	pscBuffer.dirLight[0].direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	pscBuffer.dirLight[0].specular = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);

	pscBuffer.dirLight[1].ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	pscBuffer.dirLight[1].diffuse = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	pscBuffer.dirLight[1].direction = DirectX::XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);
	pscBuffer.dirLight[1].specular = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	pscBuffer.dirLight[2].ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	pscBuffer.dirLight[2].diffuse = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	pscBuffer.dirLight[2].direction = DirectX::XMFLOAT3(0.0f, -0.707f, -0.707f);
	pscBuffer.dirLight[2].specular = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);


	VertexBuffer* pVB = new VertexBuffer(gfx, vertices, L"MirrorRoom.");
	AddBind(pVB);

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(pTopology);



	VertexConstantBuffer<CB_VS_Transform>* pVCBPerObject =
		new VertexConstantBuffer<CB_VS_Transform>(gfx, transformMatrices, 0u, 1u);
	pCopyVCBMatricesMirror = pVCBPerObject->GetVertexConstantBuffer(); //for updating every frame
	AddBind(pVCBPerObject);

	PixelShaderConstantBuffer<CB_PS_MirrorRoom>* pPSCBPerFrame =
		new PixelShaderConstantBuffer<CB_PS_MirrorRoom>(gfx, pscBuffer, 0u, 1u,
														D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyMirrorCBuffer = pPSCBPerFrame->GetPixelShaderConstantBuffer();
	AddBind(pPSCBPerFrame);

	PixelShaderConstantBuffer<TextureSwitcher>* pTexSwitcher =
		new PixelShaderConstantBuffer<TextureSwitcher>(gfx, tSwitcher, 1u, 1u,
			D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyTexSwitcher = pTexSwitcher->GetPixelShaderConstantBuffer();
	AddBind(pTexSwitcher);


	std::wstring directory[3];
	directory[0] = L"Textures\\brick01.dds";
	directory[1] = L"Textures\\ice.dds";
	directory[2] = L"Textures\\checkboard.dds";

	ShaderResourceView* pSRV = new ShaderResourceView(gfx, directory, 0u ,(UINT)std::size(directory), ShaderType::Pixel);
	AddBind(pSRV);

	TextureSampler* pTexSampler = new TextureSampler(gfx, ShaderType::Pixel);
	AddBind(pTexSampler);

}

void MirrorRoom::UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj)
{

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBMatricesMirror, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CB_VS_Transform* pMatrices = reinterpret_cast<CB_VS_Transform*>(mappedData.pData);
	pMatrices->world = in_world;
	pMatrices->worldInvTranspose = MathHelper::InverseTranspose(in_world);
	pMatrices->worldViewProjection = DirectX::XMMatrixTranspose(in_world * in_ViewProj);
	pMatrices->texTransform = DirectX::XMMatrixIdentity();
	gfx.pgfx_pDeviceContext->Unmap(pCopyVCBMatricesMirror, 0u);
}

void MirrorRoom::UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPositon)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyMirrorCBuffer, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CB_PS_MirrorRoom* frameMirror = reinterpret_cast<CB_PS_MirrorRoom*> (mappedData.pData);
	frameMirror->cameraPosition = camPositon;
	if (GetAsyncKeyState('0') & 0x8000)
		frameMirror->numberOfLights = 0;
	if (GetAsyncKeyState('1') & 0x8000)
		frameMirror->numberOfLights = 1;

	if (GetAsyncKeyState('2') & 0x8000)
		frameMirror->numberOfLights = 2;

	if (GetAsyncKeyState('3') & 0x8000)
		frameMirror->numberOfLights = 3;
	gfx.pgfx_pDeviceContext->Unmap(pCopyMirrorCBuffer, 0u);

}

void MirrorRoom::SwitchTexture(Graphics& gfx, UINT texArrPos)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyTexSwitcher, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	TextureSwitcher* frameMirror = reinterpret_cast<TextureSwitcher*> (mappedData.pData);
	frameMirror->currentTexture = texArrPos;

	if (texArrPos == 0)
	{
		frameMirror->mat = wallMaterial;
	}
	if (texArrPos == 1)
	{
		frameMirror->mat = mirrorMaterial;
	}
	if (texArrPos == 2)
	{
		frameMirror->mat = floorMaterial;
	}

	gfx.pgfx_pDeviceContext->Unmap(pCopyTexSwitcher, 0u);
}
