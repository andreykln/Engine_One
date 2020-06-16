#include "Box.h"
#include "MathHelper.h"

#define RANDFLOAT float(rand()) / 1.0f

Box::Box(Graphics& gfx, float width, float height, float depth)
{
	box.CreateBox(width, height, depth, mesh);
	std::vector<Vertex_B> vertices(mesh.vertices.size());

	for (UINT i = 0; i < mesh.vertices.size(); i++)
	{
		DirectX::XMFLOAT3 p = mesh.vertices[i].position;
		DirectX::XMFLOAT3 n = mesh.vertices[i].normal;
		vertices[i].pos = p;
 		vertices[i].normal = n;
	}
// 	constLights.objectMaterial.ambient = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
// 	constLights.objectMaterial.diffuse = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
// 	constLights.objectMaterial.specular = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);


	constLights.objectMaterial.ambient = DirectX::XMFLOAT4(0.651f, 0.5f, 0.392f, 1.0f);
	constLights.objectMaterial.diffuse = DirectX::XMFLOAT4(0.651f, 0.5f, 0.392f, 1.0f);
	constLights.objectMaterial.specular = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);

	constLights.dirLight[0].ambient = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	constLights.dirLight[0].diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	constLights.dirLight[0].direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	constLights.dirLight[0].specular = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);

	constLights.dirLight[1].ambient = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	constLights.dirLight[1].diffuse = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	constLights.dirLight[1].direction = DirectX::XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);
	constLights.dirLight[1].specular = DirectX::XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);

	constLights.dirLight[2].ambient = DirectX::XMFLOAT4(0.0, 0.0f, 0.0f, 1.0f);
	constLights.dirLight[2].diffuse = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	constLights.dirLight[2].direction = DirectX::XMFLOAT3(0.0f, -0.707f, -0.707f);
	constLights.dirLight[2].specular = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	VertexBuffer* pVB = new VertexBuffer(gfx, vertices, L"Box");
	AddBind(pVB);

	VertexShader* pVertexShader = new VertexShader(gfx, L"Shaders\\Vertex\\CubeLightTexVS.cso");
	ID3DBlob* pVertexShaderBlob = pVertexShader->GetByteCode();
	AddBind(pVertexShader); 

	
 	const UINT offset = sizeof(DirectX::XMFLOAT3);


	const std::vector<D3D11_INPUT_ELEMENT_DESC> inputElemDesc =
	{
		{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"Normal", 0u, DXGI_FORMAT_R8G8B8A8_UNORM, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"TexCoordinate", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u}
	};

	InputLayout* pInputLayout = new InputLayout(gfx, pVertexShaderBlob, inputElemDesc, L"PositionAndColor");
	AddBind(pInputLayout);

	PixelShader* pPixelShader = new PixelShader(gfx, L"Shaders\\Pixel\\CubeLightTexPS.cso");
	AddBind(pPixelShader);

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, mesh.indices, L"BoxIndexBuffer");
	AddIndexBuffer(pIndexBuffer);

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(pTopology);

// 	VertexConstantBuffer<DirectX::XMMATRIX>* pVCB =
// 		new VertexConstantBuffer<DirectX::XMMATRIX>(gfx, GetTransform() * gfx.GetProjection(), 0u, 1u);
// 	pCopyVertexConstantBuffer = pVCB->GetVertexConstantBuffer(); //for updating every frame
// 	AddBind(pVCB);



	VertexConstantBuffer<CBPerObjectTexture>* pVCBPerObject =
		new VertexConstantBuffer<CBPerObjectTexture>(gfx, constMatrices, 0u, 1u);
	pCopyVCBMatricesBox = pVCBPerObject->GetVertexConstantBuffer(); //for updating every frame
	AddBind(pVCBPerObject);

	PixelShaderConstantBuffer<CBPerFrame>* pPSCBPerFrame =
		new PixelShaderConstantBuffer<CBPerFrame>(gfx, constLights, 1u, 1u);
	pCopyPCBLightsBox = pPSCBPerFrame->GetPixelShaderConstantBuffer();
	AddBind(pPSCBPerFrame);

// 	RasterizerState state;
// 	Rasterizer* pRasterState = new Rasterizer(gfx, state.Wireframe());
// 	AddBind(pRasterState);

	ShaderResourceView* pSRV = new ShaderResourceView(gfx, L"Textures\\WoodCrate01.dds");

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
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBMatricesBox, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CBPerObjectTexture* object = reinterpret_cast<CBPerObjectTexture*>(mappedData.pData);
	object->gWorld = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
	object->gWorldInvTranspose = MathHelper::InverseTranspose(object->gWorld);
	object->gWorldViewProj = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
	object->gTexTransform = DirectX::XMMatrixIdentity();
	gfx.pgfx_pDeviceContext->Unmap(pCopyVCBMatricesBox, 0u);

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


