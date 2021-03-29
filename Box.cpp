#include "Box.h"
#include "MathHelper.h"

#define RANDFLOAT float(rand()) / 1.0f

Box::Box(Graphics& gfx, float width, float height, float depth, DemoSwitch demo)
	: currentDemo{ demo }
{

	box.CreateBox(width, height, depth, mesh);
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

	if (currentDemo == DemoSwitch::Shapesdemo)
	{
		constLights.objectMaterial.ambient = DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
		constLights.objectMaterial.diffuse = DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
		constLights.objectMaterial.specular = DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 16.0f);

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
	} 
	if ( (currentDemo == DemoSwitch::HillsDemo))
	{
		constLights.objectMaterial.ambient = DirectX::XMFLOAT4(0.651f, 0.5f, 0.392f, 1.0f);
		constLights.objectMaterial.diffuse = DirectX::XMFLOAT4(0.651f, 0.5f, 0.392f, 1.0f);
		constLights.objectMaterial.specular = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);

		constLights.dirLight[0].ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		constLights.dirLight[0].diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		constLights.dirLight[0].direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
		constLights.dirLight[0].specular = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);

		constLights.dirLight[1].ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
		constLights.dirLight[1].diffuse = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
		constLights.dirLight[1].direction = DirectX::XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);
		constLights.dirLight[1].specular = DirectX::XMFLOAT4(0.55f, 0.55f, 0.55f, 1.0f);

		constLights.dirLight[2].ambient = DirectX::XMFLOAT4(0.5, 0.5f, 0.5f, 1.0f);
		constLights.dirLight[2].diffuse = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
		constLights.dirLight[2].direction = DirectX::XMFLOAT3(0.0f, -0.707f, -0.707f);
		constLights.dirLight[2].specular = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	}


	VertexBuffer* pVB = new VertexBuffer(gfx, vertices, L"Box.");
	AddBind(pVB);

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, mesh.indices, L"BoxIndexBuffer.");
	AddIndexBuffer(pIndexBuffer);

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(pTopology);

	VertexConstantBuffer<CB_VS_Transform>* pVCBPerObject =
		new VertexConstantBuffer<CB_VS_Transform>(gfx, transformMatrices, 0u, 1u);
	pCopyVCBMatricesBox = pVCBPerObject->GetVertexConstantBuffer(); //for updating every frame
	AddBind(pVCBPerObject);

	PixelShaderConstantBuffer<CBPerFrame>* pPSCBPerFrame =
		new PixelShaderConstantBuffer<CBPerFrame>(gfx, constLights, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyPCBLightsBox = pPSCBPerFrame->GetPixelShaderConstantBuffer();
	AddBind(pPSCBPerFrame);

	PixelShaderConstantBuffer<CBFog>* pFog =
		new PixelShaderConstantBuffer<CBFog>(gfx, fogObj, 1u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	AddBind(pFog);

	std::wstring directory[1];
	if (currentDemo == DemoSwitch::Shapesdemo)
	{
		directory[0] = L"Textures\\LightGreenMarble.dds";
	}
	if (currentDemo == DemoSwitch::HillsDemo)
	{
		directory[0] = L"Textures\\WireFence.dds";
	}
	if (currentDemo == DemoSwitch::DefaultBox)
	{
		directory[0] = L"Textures\\WoodCrate01.dds";
	}

	ShaderResourceView* pSRV = new ShaderResourceView(gfx, directory, (UINT)std::size(directory));
	AddBind(pSRV);

	TextureSampler* pTexSampler = new TextureSampler(gfx);
	AddBind(pTexSampler);

}

DirectX::XMMATRIX Box::GetTransform() const noexcept
{
	return m_Matrix * m_Centered;
}

void Box::Update(float dt) noexcept
{
	alpha = dt;
	
}

void Box::UpdateVertexConstantBuffer(Graphics& gfx)
{


	D3D11_MAPPED_SUBRESOURCE mappedData;

	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPCBLightsBox, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CBPerFrame* frame = reinterpret_cast<CBPerFrame*> (mappedData.pData);
	
	if (GetAsyncKeyState('0') & 0x8000)
		frame->numLights = 0;
	if (GetAsyncKeyState('1') & 0x8000)
		frame->numLights = 1;

	if (GetAsyncKeyState('2') & 0x8000)
		frame->numLights = 2;

	if (GetAsyncKeyState('3') & 0x8000)
		frame->numLights = 3;
	gfx.pgfx_pDeviceContext->Unmap(pCopyPCBLightsBox, 0u);
}

DirectX::XMMATRIX Box::GetBoxForHillsOffset()
{
	return boxforHillsOffset;
}

void Box::UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj)
{

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBMatricesBox, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CB_VS_Transform* pMatrices = reinterpret_cast<CB_VS_Transform*>(mappedData.pData);
	pMatrices->world = in_world;
	pMatrices->worldInvTranspose = MathHelper::InverseTranspose(in_world);
	pMatrices->worldViewProjection = DirectX::XMMatrixTranspose(in_world * in_ViewProj);
	pMatrices->texTransform = DirectX::XMMatrixIdentity();
	gfx.pgfx_pDeviceContext->Unmap(pCopyVCBMatricesBox, 0u);

}

