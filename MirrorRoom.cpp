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

	mirrorMaterial.ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mirrorMaterial.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);
	mirrorMaterial.specular = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 16.0f);

	wallMaterial.ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	wallMaterial.diffuse = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	wallMaterial.specular = DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 16.0f);


	constLights.dirLight[0].ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	constLights.dirLight[0].diffuse = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	constLights.dirLight[0].direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	constLights.dirLight[0].specular = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);

	constLights.dirLight[1].ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	constLights.dirLight[1].diffuse = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	constLights.dirLight[1].direction = DirectX::XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);
	constLights.dirLight[1].specular = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	constLights.dirLight[2].ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	constLights.dirLight[2].diffuse = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	constLights.dirLight[2].direction = DirectX::XMFLOAT3(0.0f, -0.707f, -0.707f);
	constLights.dirLight[2].specular = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);


	VertexBuffer* pVB = new VertexBuffer(gfx, vertices, L"MirrorRoom.");
	AddBind(pVB);

	VertexShader* pVertexShader = new VertexShader(gfx, L"Shaders\\Vertex\\LightAndTextureVS.cso");
	ID3DBlob* pVertexShaderBlob = pVertexShader->GetByteCode();
	AddBind(pVertexShader);

	InputLayout* pInputLayout = new InputLayout(gfx, pVertexShaderBlob, inputLightTexture, L"PositionAndColor.");
	AddBind(pInputLayout);

	PixelShader* pPixelShader = new PixelShader(gfx, L"Shaders\\Pixel\\MirrorRoomPS.cso");
	AddBind(pPixelShader);

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(pTopology);



	VertexConstantBuffer<CBPerObjectTexture>* pVCBPerObject =
		new VertexConstantBuffer<CBPerObjectTexture>(gfx, constMatrices, 0u, 1u);
	pCopyVCBMatricesMirror = pVCBPerObject->GetVertexConstantBuffer(); //for updating every frame
	AddBind(pVCBPerObject);

	PixelShaderConstantBuffer<CBPerFrameMirrorRoom>* pPSCBPerFrame =
		new PixelShaderConstantBuffer<CBPerFrameMirrorRoom>(gfx, constLights, 0u, 1u,
			static_cast<D3D11_CPU_ACCESS_FLAG>(0u), D3D11_USAGE_IMMUTABLE);
	pCopyPCBLightsMirror = pPSCBPerFrame->GetPixelShaderConstantBuffer();
	AddBind(pPSCBPerFrame);


	PixelShaderConstantBuffer<MirrorRoomCB>* pMirrorRoom =
		new PixelShaderConstantBuffer<MirrorRoomCB>(gfx, testCB, 1u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyMirrorRoomCB = pMirrorRoom->GetPixelShaderConstantBuffer();
	AddBind(pMirrorRoom);

	PixelShaderConstantBuffer<LightSwitcher>* pLightSwitcher =
		new PixelShaderConstantBuffer<LightSwitcher>(gfx, switcher, 2u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyLightSwitcher = pLightSwitcher->GetPixelShaderConstantBuffer();
	AddBind(pLightSwitcher);


// 	PixelShaderConstantBuffer<CBFog>* pFog =
// 		new PixelShaderConstantBuffer<CBFog>(gfx, fogObj, 1u, 1u);
// 	AddBind(pFog);


// 	RasterizerState state;
// 	Rasterizer* pRasterState = new Rasterizer(gfx, state.CullBackNone());
// 	AddBind(pRasterState);


	std::wstring directory[3];
	directory[0] = L"Textures\\brick01.dds";
	directory[1] = L"Textures\\ice.dds";
	directory[2] = L"Textures\\checkboard.dds";

	ShaderResourceView* pSRV = new ShaderResourceView(gfx, directory, (UINT)std::size(directory));
	AddBind(pSRV);

	TextureSampler* pTexSampler = new TextureSampler(gfx);
	AddBind(pTexSampler);

// 	Blending* pBlending = new Blending(gfx, D3D11_COLOR_WRITE_ENABLE_ALL, FALSE);
// 	AddBind(pBlending);

}

DirectX::XMMATRIX MirrorRoom::GetTransform() const noexcept
{
	return m_Matrix * m_Centered;
}

void MirrorRoom::Update(float dt) noexcept
{
	alpha = dt;
}

void MirrorRoom::UpdateVertexConstantBuffer(Graphics& gfx)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBMatricesMirror, 0u, D3D11_MAP_WRITE, 0u, &mappedData));
	CBPerObjectTexture* object = reinterpret_cast<CBPerObjectTexture*>(mappedData.pData);
	object->gWorld = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
	object->gWorldInvTranspose = MathHelper::InverseTranspose(object->gWorld);
	object->gWorldViewProj = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
	object->gTexTransform = DirectX::XMMatrixIdentity();
	gfx.pgfx_pDeviceContext->Unmap(pCopyVCBMatricesMirror, 0u);

	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPCBLightsMirror, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CBPerFrame* frame = reinterpret_cast<CBPerFrame*> (mappedData.pData);

	if (GetAsyncKeyState('0') & 0x8000)
		frame->numLights = 0;
	if (GetAsyncKeyState('1') & 0x8000)
		frame->numLights = 1;

	if (GetAsyncKeyState('2') & 0x8000)
		frame->numLights = 2;

	if (GetAsyncKeyState('3') & 0x8000)
		frame->numLights = 3;
	gfx.pgfx_pDeviceContext->Unmap(pCopyPCBLightsMirror, 0u);
}

void MirrorRoom::UpdateMirrorRoomConstBuffers(Graphics& gfx, UINT texture)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBMatricesMirror, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CBPerObjectTexture* object = reinterpret_cast<CBPerObjectTexture*>(mappedData.pData);
	object->gWorld = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
	object->gWorldInvTranspose = MathHelper::InverseTranspose(object->gWorld);
	object->gWorldViewProj = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
	object->gTexTransform = DirectX::XMMatrixIdentity();
	gfx.pgfx_pDeviceContext->Unmap(pCopyVCBMatricesMirror, 0u);

	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyMirrorRoomCB, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	MirrorRoomCB* frameMirror = reinterpret_cast<MirrorRoomCB*> (mappedData.pData);
	if (texture == 0)
	{
		frameMirror->objectMaterial = wallMaterial;
	}
	if (texture == 1)
	{
		frameMirror->objectMaterial = mirrorMaterial;
	}
	if (texture == 2)
	{
		frameMirror->objectMaterial = floorMaterial;
	}
	//switch texture each draw
	frameMirror->currentTexture = texture;
	gfx.pgfx_pDeviceContext->Unmap(pCopyMirrorRoomCB, 0u);

	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyLightSwitcher, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	LightSwitcher* temptLightSwitcher = reinterpret_cast<LightSwitcher*> (mappedData.pData);
	if (GetAsyncKeyState('0') & 0x8000)
		temptLightSwitcher->numLights = 0;
	if (GetAsyncKeyState('1') & 0x8000)
		temptLightSwitcher->numLights = 1;
	if (GetAsyncKeyState('2') & 0x8000)
		temptLightSwitcher->numLights = 2;
	if (GetAsyncKeyState('3') & 0x8000)
		temptLightSwitcher->numLights = 3;
	gfx.pgfx_pDeviceContext->Unmap(pCopyLightSwitcher, 0u);

}