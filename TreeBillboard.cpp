#include "TreeBillboard.h"

TreeBillboard::TreeBillboard(Graphics& gfx)
{
	std::vector<TreePointSprite> treePositions;
	treePositions.resize(25);
	MathHelper helper;
	for (auto& a : treePositions)
	{
		a.pos.x = helper.RandomFloatWithinRange(-75.0f, 75.0f);
		a.pos.z = helper.RandomFloatWithinRange(-75.0f, 75.0f);
		a.pos.y = (0.3f * (a.pos.z * sinf(0.1f * a.pos.x) + a.pos.x * cosf(0.1f * a.pos.z))) + 10.0f; //GetHeight function from Hills
		a.size = DirectX::XMFLOAT2{ 25.0f, 25.0f };
	}


	VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, treePositions, L"TreeSpritePositions");
	AddBind(pVertexBuffer);

	GeometryShaderConstantBuffer<CBBillboardGeometry>* pGSconstBuffer =
		new GeometryShaderConstantBuffer<CBBillboardGeometry>(gfx, CBGeometryShader, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyGSConstBuffer = pGSconstBuffer->GetGeometryShaderConstantBuffer();
	AddBind(pGSconstBuffer);

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
	directionalLight.mat.diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	directionalLight.mat.reflect = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	directionalLight.mat.specular = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);

	std::wstring treesTex[4];
	for (UINT i = 0; i < 4; ++i)
	{
		treesTex[i] = L"Textures\\Trees\\tree" + std::to_wstring(i + 1) + L".dds";
	}
	ShaderResourceView* pSRV = new ShaderResourceView(gfx, treesTex, (UINT)std::size(treesTex), 1, true);
	AddBind(pSRV);

	PixelShaderConstantBuffer<CB_PS_DirectionalL_Fog>* pPSconstBuffer =
		new PixelShaderConstantBuffer<CB_PS_DirectionalL_Fog>(gfx, directionalLight, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	AddBind(pPSconstBuffer);

	PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>* pPSPerFrame =
		new PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>(gfx, pscBuffer, 1u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyPSConstBuffer = pPSPerFrame->GetPixelShaderConstantBuffer();
	AddBind(pPSPerFrame);

	TextureSampler* pTexSampler = new TextureSampler(gfx);
	AddBind(pTexSampler);

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	AddBind(pTopology);
}



void TreeBillboard::UpdateConstantBuffers(Graphics& gfx, const DirectX::XMMATRIX& in_ViewProj, DirectX::XMFLOAT3 cameraPosition)
{
	UpdateGSConstBuffer(gfx, in_ViewProj, cameraPosition);
	UpdatePSConstBuffers(gfx, cameraPosition);
}

void TreeBillboard::UpdateGSConstBuffer(Graphics& gfx, const DirectX::XMMATRIX& in_ViewProj, DirectX::XMFLOAT3 cameraPosition)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyGSConstBuffer, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CBBillboardGeometry* geometry = reinterpret_cast<CBBillboardGeometry*> (mappedData.pData);
	geometry->worldViewProjection = DirectX::XMMatrixTranspose(in_ViewProj);
	geometry->cameraPosition = cameraPosition;
	gfx.pgfx_pDeviceContext->Unmap(pCopyGSConstBuffer, 0u);
}

void TreeBillboard::UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 cameraPosition)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPSConstBuffer, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CB_PS_PerFrameUpdate* pixel = reinterpret_cast<CB_PS_PerFrameUpdate*> (mappedData.pData);
	pixel->cameraPositon = cameraPosition;
	if (GetAsyncKeyState('0') & 0x8000)
		pixel->numberOfLights = 0;
	if (GetAsyncKeyState('1') & 0x8000)
		pixel->numberOfLights = 1;
	if (GetAsyncKeyState('2') & 0x8000)
		pixel->numberOfLights = 2;
	if (GetAsyncKeyState('3') & 0x8000)
		pixel->numberOfLights = 3;
	gfx.pgfx_pDeviceContext->Unmap(pCopyPSConstBuffer, 0u);
}
