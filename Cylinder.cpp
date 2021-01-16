#include "Cylinder.h"
Cylinder::Cylinder(Graphics& gfx,
	float bottom_radius, float top_radius, float height, UINT slice_count, UINT stack_count, bool caps, bool lightning)
{
	if (caps)
	{
		cylinderParts.CreateCylinder(bottom_radius, top_radius, height, slice_count, stack_count, mesh);
	}
	else
	{
		cylinderParts.CreateCylinderNoCaps(bottom_radius, top_radius, height, slice_count, stack_count, mesh);
	}
	std::vector<Vertex_IA> vertices(mesh.vertices.size());

	for (UINT i = 0; i < mesh.vertices.size(); i++)
	{
		DirectX::XMFLOAT3 p = mesh.vertices[i].position;
		DirectX::XMFLOAT3 n = mesh.vertices[i].normal;
		DirectX::XMFLOAT2 t = mesh.vertices[i].TexC;

		vertices[i].pos = p;
		vertices[i].normal = n;
		vertices[i].tex = t;
	}

	constLights.objectMaterial.ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	constLights.objectMaterial.diffuse = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	constLights.objectMaterial.specular = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);

	constLights.dirLight[0].ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	constLights.dirLight[0].diffuse = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	constLights.dirLight[0].direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	constLights.dirLight[0].specular = DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);

	constLights.dirLight[1].ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	constLights.dirLight[1].diffuse = DirectX::XMFLOAT4(0.55f, 0.55f, 0.55f, 1.0f);
	constLights.dirLight[1].direction = DirectX::XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);
	constLights.dirLight[1].specular = DirectX::XMFLOAT4(0.05f, 0.05f, 0.05f, 1.0f);

	constLights.dirLight[2].ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	constLights.dirLight[2].diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	constLights.dirLight[2].direction = DirectX::XMFLOAT3(0.0f, -0.707f, -0.707f);
	constLights.dirLight[2].specular = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);


	VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, vertices, L"Cylinder");
	AddBind(pVertexBuffer);

	VertexShader* pVertexShader = new VertexShader(gfx, L"Shaders\\Vertex\\LightAndTextureVS.cso");
	ID3DBlob* pVertexShaderBlob = pVertexShader->GetByteCode();
	AddBind(pVertexShader);

	InputLayout* pInputLayout = new InputLayout(gfx, pVertexShaderBlob, inputLightTexture, L"PositionAndColor");
	AddBind(pInputLayout);


	PixelShader* pPixelShader = new PixelShader(gfx, L"Shaders\\Pixel\\LightAndTexturePS.cso");
	AddBind(pPixelShader);

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, mesh.indices, L"CylinderIndexBuffer");
	AddIndexBuffer(pIndexBuffer);

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(pTopology);

	VertexConstantBuffer<CBPerObjectTexture>* pVCBPerObject =
		new VertexConstantBuffer<CBPerObjectTexture>(gfx, constMatrices, 0u, 1u);
	pCopyVCBMatricesCylinder = pVCBPerObject->GetVertexConstantBuffer(); //for updating every frame
	AddBind(pVCBPerObject);

	PixelShaderConstantBuffer<CBPerFrame>* pPSCBPerFrame =
		new PixelShaderConstantBuffer<CBPerFrame>(gfx, constLights, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyPCBLightsCylinder = pPSCBPerFrame->GetPixelShaderConstantBuffer();
	AddBind(pPSCBPerFrame);
	////////////////////////////////////////////////////////////
	// 	directory[0] = L"Textures\\flare.dds";
  	std::wstring directoryL[60];

	//std::wstring fireAnimArray[120];
// 	for (UINT i = 0; i < 120; ++i)
// 	{
// 		directory[i] = L"Textures\\FireAnim\\Fire" + std::to_wstring(i+ 1) + L".dds";
// 	}
	for (UINT i = 0; i < 60; ++i)
	{
		directoryL[i] = L"Textures\\Lightning\\Spark" + std::to_wstring(i + 1) + L".dds";
	}


	//////////////////////////////////////////////////////////////
	std::wstring directory[1];
	if (lightning)
	{
		directory[0] = L"Textures\\Lightning\\Spark1.dds";
	}
	else
	{
		directory[0] = L"Textures\\brick01.dds";
	}
	ShaderResourceView* pSRV = new ShaderResourceView(gfx, directoryL, (UINT)std::size(directoryL));
	AddBind(pSRV);

	TextureSampler* pTexSampler = new TextureSampler(gfx);
	AddBind(pTexSampler);

	PixelShaderConstantBuffer<CBFog>* pFog =
		new PixelShaderConstantBuffer<CBFog>(gfx, fogObj, 1u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	AddBind(pFog);

// 	Blending* pBlending = new Blending(gfx, D3D11_COLOR_WRITE_ENABLE_ALL, FALSE);
// 	AddBind(pBlending);
// 	RasterizerState state;
// 	Rasterizer* pRasterState = new Rasterizer(gfx, state.SolidFill());
// 	AddBind(pRasterState);

}

DirectX::XMMATRIX Cylinder::GetTransform() const noexcept
{
	return m_Matrix * m_Centered;
}

void Cylinder::Update(float dt) noexcept
{
	alpha = dt;
}

void Cylinder::UpdateVertexConstantBuffer(Graphics& gfx)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBMatricesCylinder, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CBPerObjectTexture* object = reinterpret_cast<CBPerObjectTexture*>(mappedData.pData);
	object->gWorld = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
	object->gWorldInvTranspose = MathHelper::InverseTranspose(object->gWorld);
	object->gWorldViewProj = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
	object->gTexTransform = DirectX::XMMatrixIdentity();
	gfx.pgfx_pDeviceContext->Unmap(pCopyVCBMatricesCylinder, 0u);

	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPCBLightsCylinder, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CBPerFrame* frame = reinterpret_cast<CBPerFrame*> (mappedData.pData);

	if (GetAsyncKeyState('0') & 0x8000)
		frame->numLights = 0;
	if (GetAsyncKeyState('1') & 0x8000)
		frame->numLights = 1;

	if (GetAsyncKeyState('2') & 0x8000)
		frame->numLights = 2;

	if (GetAsyncKeyState('3') & 0x8000)
		frame->numLights = 3;
	gfx.pgfx_pDeviceContext->Unmap(pCopyPCBLightsCylinder, 0u);
}

