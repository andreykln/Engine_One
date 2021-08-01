#include "DisplacementWaves.h"

DisplacementWaves::DisplacementWaves(Graphics& gfx)
{
	geoGen.CreateGrid(25.0f, 25.0f, 65, 45, mesh);
	std::vector<Vertices_Full> vertices(mesh.vertices.size());

	cbVSPerObject.dirLight[0].ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	cbVSPerObject.dirLight[0].diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	cbVSPerObject.dirLight[0].direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	cbVSPerObject.dirLight[0].specular = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	cbVSPerObject.dirLight[1].ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	cbVSPerObject.dirLight[1].diffuse = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	cbVSPerObject.dirLight[1].direction = DirectX::XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);
	cbVSPerObject.dirLight[1].specular = DirectX::XMFLOAT4(0.35f, 0.35f, 0.35f, 1.0f);

	cbVSPerObject.dirLight[2].ambient = DirectX::XMFLOAT4(0.5, 0.5f, 0.5f, 1.0f);
	cbVSPerObject.dirLight[2].diffuse = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	cbVSPerObject.dirLight[2].direction = DirectX::XMFLOAT3(0.0f, -0.707f, -0.707f);
	cbVSPerObject.dirLight[2].specular = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	cbVSPerObject.mat.ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	cbVSPerObject.mat.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);
	cbVSPerObject.mat.specular = DirectX::XMFLOAT4(0.9f, 0.9f, 0.9f, 128.0f);
	cbVSPerObject.mat.reflect = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	for (int i = 0; i < mesh.vertices.size(); ++i)
	{
		vertices[i].pos = mesh.vertices[i].position;
		vertices[i].normal = DirectX::XMFLOAT3{ 0.0f, 1.0f, 0.0f };
		vertices[i].tex = mesh.vertices[i].TexC;
		vertices[i].tangent = mesh.vertices[i].tangentU;
	}

	VertexBuffer* pVB = new VertexBuffer(gfx, vertices, L"DisplacementWaves");
	AddBind(pVB);

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, mesh.indices, L"DisplWavesIndexBuffer");
	AddIndexBuffer(pIndexBuffer);

	PixelShaderConstantBuffer<CB_PS_DirectionalL_Fog>* pVSCB =
		new PixelShaderConstantBuffer<CB_PS_DirectionalL_Fog>(gfx, cbVSPerObject, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	AddBind(pVSCB);

	PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>* pPSCB =
		new PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>(gfx, cbPSPerFrame, 1u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyPSCBPerFrame = pPSCB->GetPixelShaderConstantBuffer();
	AddBind(pPSCB);


	VertexConstantBuffer<CB_VS_DisplacementWavesPerFrame>* pVSCBPerFrame =
		new VertexConstantBuffer<CB_VS_DisplacementWavesPerFrame>(gfx, cbVSPerFrame, 0u, 1u);
	pCopyVSCBPerFrame = pVSCBPerFrame->GetVertexConstantBuffer();
	AddBind(pVSCBPerFrame);

	DomainShaderConstantBuffer<CB_CameraPosition_ViewProj>* pDSCBPerFrame =
		new DomainShaderConstantBuffer<CB_CameraPosition_ViewProj>(gfx, cbDSCameraViewProj, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyDSCBPerFrame = pDSCBPerFrame->GetDomainShaderConstantBuffer();
	AddBind(pDSCBPerFrame);


	std::wstring waterTexture[1];
	const wchar_t* filePath = L"Textures\\water1.dds";
	waterTexture[0] = L"Textures\\water1.dds";
	//diffuse map to PS
	ShaderResourceView* pSRV0 = new ShaderResourceView(gfx, waterTexture, 0u, 1u, ShaderType::Pixel);
	AddBind(pSRV0);

	filePath = L"Textures\\waves0.dds";
	//normal map to pixel
	ShaderResourceView* pSRV1 = new ShaderResourceView(gfx, filePath);
	pSRVPSNormalMap0 = pSRV1->GetSRV();
	//height map to domain
	ShaderResourceView* pSRV2 = new ShaderResourceView(gfx, filePath);
	pSRVDSHeightMap0 = pSRV2->GetSRV();

	filePath = L"Textures\\waves1.dds";
	//normal map 2 to pixel
	ShaderResourceView* pSRV3 = new ShaderResourceView(gfx, filePath);
	pSRVPSNormalMap1 = pSRV3->GetSRV();
	//height map 2 to domain
	ShaderResourceView* pSRV4 = new ShaderResourceView(gfx, filePath);
	pSRVDSHeightMap1 = pSRV4->GetSRV();


	TextureSampler* pTexSampler = new TextureSampler(gfx, ShaderType::Pixel);
	AddBind(pTexSampler);

	TextureSampler* pDomainTexSampler = new TextureSampler(gfx, ShaderType::Domain);
	AddBind(pDomainTexSampler);



}

void DisplacementWaves::UpdateCBs(Graphics& gfx, DirectX::XMMATRIX in_world, DirectX::XMMATRIX viewProjection,
	DirectX::XMFLOAT3 cameraPosition, float dt)
{


	waveDispOffset0.x += 0.01f * dt;
	waveDispOffset0.y += 0.03f * dt;

	waveDispOffset1.x += 0.01f * dt;
	waveDispOffset1.y += 0.03f * dt;

	DirectX::XMMATRIX waveScale0 = DirectX::XMMatrixScaling(2.0f, 2.0f, 1.0f);
	DirectX::XMMATRIX waveOffset0 = DirectX::XMMatrixTranslation(waveDispOffset0.x, waveDispOffset0.y, 0.0f);
	waveDispTexTransform0 = DirectX::XMMatrixMultiply(waveScale0, waveOffset0);

	DirectX::XMMATRIX waveScale1 = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	DirectX::XMMATRIX waveOffset1 = DirectX::XMMatrixTranslation(waveDispOffset1.x, waveDispOffset1.y, 0.0f);
	waveDispTexTransform1 = DirectX::XMMatrixMultiply(waveScale1, waveOffset1);

	waveNormalOffset0.x += 0.05f * dt;
	waveNormalOffset0.y += 0.2f * dt;

	waveNormalOffset1.x += 0.02f * dt;
	waveNormalOffset1.y += 0.05f * dt;

	waveScale0 = DirectX::XMMatrixScaling(22.0f, 22.0f, 1.0f);
	waveOffset0 = DirectX::XMMatrixTranslation(waveNormalOffset0.x, waveNormalOffset0.y, 0.0f);
	waveNormalTransform0 = DirectX::XMMatrixMultiply(waveScale0, waveOffset0);

	waveScale1 = DirectX::XMMatrixScaling(16.0f, 16.0f, 1.0f);
	waveOffset1 = DirectX::XMMatrixTranslation(waveNormalOffset1.x, waveNormalOffset1.y, 0.0f);
	waveNormalTransform1 = DirectX::XMMatrixMultiply(waveScale1, waveOffset1);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVSCBPerFrame, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CB_VS_DisplacementWavesPerFrame* pVSCB = reinterpret_cast<CB_VS_DisplacementWavesPerFrame*>(mappedData.pData);
	pVSCB->cameraPosition = cameraPosition;
	pVSCB->texTransform = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	pVSCB->world = DirectX::XMMatrixTranspose(in_world);
	pVSCB->worldInvTranspose = MathHelper::InverseTranspose(in_world);
	pVSCB->worldViewProjection = DirectX::XMMatrixTranspose(in_world * viewProjection);
	pVSCB->waveDispTexTransform0 = DirectX::XMMatrixTranspose(waveDispTexTransform0);
	pVSCB->waveDispTexTransform1 = DirectX::XMMatrixTranspose(waveDispTexTransform1);
	pVSCB->waveNormalTexTransform0 = DirectX::XMMatrixTranspose(waveNormalTransform0);
	pVSCB->waveNormalTexTransform1 = DirectX::XMMatrixTranspose(waveNormalTransform1);
	gfx.pgfx_pDeviceContext->Unmap(pCopyVSCBPerFrame, 0u);


	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPSCBPerFrame, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CB_PS_PerFrameUpdate* frame = reinterpret_cast<CB_PS_PerFrameUpdate*> (mappedData.pData);
	frame->cameraPositon = cameraPosition;
	if (GetAsyncKeyState('0') & 0x8000)
		frame->numberOfLights = 0;
	if (GetAsyncKeyState('1') & 0x8000)
		frame->numberOfLights = 1;
	if (GetAsyncKeyState('2') & 0x8000)
		frame->numberOfLights = 2;
	if (GetAsyncKeyState('3') & 0x8000)
		frame->numberOfLights = 3;

	gfx.pgfx_pDeviceContext->Unmap(pCopyPSCBPerFrame, 0u);

	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyDSCBPerFrame, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CB_CameraPosition_ViewProj* frameDS = reinterpret_cast<CB_CameraPosition_ViewProj*> (mappedData.pData);
	frameDS->cameraPosition = cameraPosition;
	frameDS->viewProjection = DirectX::XMMatrixTranspose(viewProjection);
	gfx.pgfx_pDeviceContext->Unmap(pCopyDSCBPerFrame, 0u);

	gfx.pgfx_pDeviceContext->PSSetShaderResources(1u, 1u, &pSRVPSNormalMap0);
	gfx.pgfx_pDeviceContext->PSSetShaderResources(2u, 1u, &pSRVPSNormalMap1);

	gfx.pgfx_pDeviceContext->DSSetShaderResources(0u, 1u, &pSRVDSHeightMap0);
	gfx.pgfx_pDeviceContext->DSSetShaderResources(1u, 1u, &pSRVDSHeightMap1);





}
