#include "TreeBillboard.h"

TreeBillboard::TreeBillboard(Graphics& gfx, std::vector<TreePointSprite> in_trees_pos)
	: treePositions(in_trees_pos)
{
	VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, treePositions, L"TreeSpritePositions");
	AddBind(pVertexBuffer);

	VertexShader* pVertexShader = new VertexShader(gfx, L"Shaders\\Vertex\\TreeBillboardVS.cso");
	pVertexShaderBlob = pVertexShader->GetByteCode();
	AddBind(pVertexShader);

	InputLayout* pInputLayout = new InputLayout(gfx, pVertexShaderBlob, treeBbrdLayout, L"TreeBilboardInLayout");
	AddBind(pInputLayout);

	GeometryShader* pGeometryShader = new GeometryShader(gfx, L"Shaders\\Geometry\\TreeBillboardGS.cso");
	AddBind(pGeometryShader);

	GeometryShaderConstantBuffer<CBBillboardGeometry>* pGSconstBuffer =
		new GeometryShaderConstantBuffer<CBBillboardGeometry>(gfx, CBGeometryShader, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyGSConstBuffer = pGSconstBuffer->GetGeometryShaderConstantBuffer();
	AddBind(pGSconstBuffer);

	dirLight_[0].ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	dirLight_[0].diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	dirLight_[0].direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	dirLight_[0].specular = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	dirLight_[1].ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	dirLight_[1].diffuse = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	dirLight_[1].direction = DirectX::XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);
	dirLight_[1].specular = DirectX::XMFLOAT4(0.35f, 0.35f, 0.35f, 1.0f);
	dirLight_[2].ambient = DirectX::XMFLOAT4(0.5, 0.5f, 0.5f, 1.0f);
	dirLight_[2].diffuse = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	dirLight_[2].direction = DirectX::XMFLOAT3(0.0f, -0.707f, -0.707f);
	dirLight_[2].specular = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	CBPixelShader.dirLight[0] = dirLight_[0];
	CBPixelShader.dirLight[1] = dirLight_[1];
	CBPixelShader.dirLight[2] = dirLight_[2];

	CBPixelShader.treeMaterial.ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	CBPixelShader.treeMaterial.diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	CBPixelShader.treeMaterial.reflect = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	CBPixelShader.treeMaterial.specular = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);

	std::wstring treesTex[4];
	for (UINT i = 0; i < 4; ++i)
	{
		treesTex[i] = L"Textures\\Trees\\tree" + std::to_wstring(i + 1) + L".dds";
	}
	ShaderResourceView* pSRV = new ShaderResourceView(gfx, treesTex, (UINT)std::size(treesTex), 1, true);
	AddBind(pSRV);

	PixelShader* pPixelShader = new PixelShader(gfx, L"Shaders\\Pixel\\TreeBillboardPS.cso");
	AddBind(pPixelShader);

	PixelShaderConstantBuffer<CBBillboardPixel>* pPSconstBuffer =
		new PixelShaderConstantBuffer<CBBillboardPixel>(gfx, CBPixelShader, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyPSConstBuffer = pPSconstBuffer->GetPixelShaderConstantBuffer();
	AddBind(pPSconstBuffer);

	TextureSampler* pTexSampler = new TextureSampler(gfx);
	AddBind(pTexSampler);

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	AddBind(pTopology);
}

DirectX::XMMATRIX TreeBillboard::GetTransform() const noexcept
{
	return m_Matrix * m_Centered;;
}

void TreeBillboard::Update(float dt) noexcept
{

}

void TreeBillboard::UpdateVertexConstantBuffer(Graphics& gfx)
{



}

void TreeBillboard::UpdateConstantBuffers(Graphics& gfx, DirectX::XMFLOAT3& eyePosition)
{
	UpdateGeometryConstBufer(gfx, eyePosition);
	UpdatePixelShaderConstBuffer(gfx);
}

void TreeBillboard::UpdateGeometryConstBufer(Graphics& gfx, DirectX::XMFLOAT3& eyePosition)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyGSConstBuffer, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CBBillboardGeometry* geometry = reinterpret_cast<CBBillboardGeometry*> (mappedData.pData);
	geometry->gWorldViewProj = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
	geometry->cbEyePosition = eyePosition;
	gfx.pgfx_pDeviceContext->Unmap(pCopyGSConstBuffer, 0u);
}

void TreeBillboard::UpdatePixelShaderConstBuffer(Graphics& gfx)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPSConstBuffer, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CBBillboardPixel* pixel = reinterpret_cast<CBBillboardPixel*> (mappedData.pData);
	pixel->treeMaterial = CBPixelShader.treeMaterial;
	if (GetAsyncKeyState('0') & 0x8000)
		pixel->numLights[0] = 0;
	if (GetAsyncKeyState('1') & 0x8000)
		pixel->numLights[0] = 1;
	if (GetAsyncKeyState('2') & 0x8000)
		pixel->numLights[0] = 2;
	if (GetAsyncKeyState('3') & 0x8000)
		pixel->numLights[0] = 3;
	gfx.pgfx_pDeviceContext->Unmap(pCopyPSConstBuffer, 0u);
}
