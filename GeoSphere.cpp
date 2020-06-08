#include "GeoSphere.h"
#include "App.h"

GeoSphere::GeoSphere(Graphics& gfx, float radius, UINT numSubdivisions)
{
	struct Vertex_G
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
	};
	sphere.CreateGeosphere(radius, numSubdivisions, mesh);
	std::vector<Vertex_G> vertices(mesh.vertices.size());
	//DirectX::XMFLOAT4 col{ 0.5f, 0.6f, 0.2f, 1.0f };
	for (UINT i = 0; i < mesh.vertices.size(); i++)
	{
		DirectX::XMFLOAT3 p = mesh.vertices[i].position;
		vertices[i].pos = p;
		vertices[i].normal = mesh.vertices[i].normal;

	}


	VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, vertices, L"GeoSphere");
	AddBind(pVertexBuffer);

	VertexShader* pVertexShader = new VertexShader(gfx, L"Shaders\\Vertex\\LightVS.cso");
	ID3DBlob* pVertexShaderBlob = pVertexShader->GetByteCode();
	AddBind(pVertexShader);

	const std::vector<D3D11_INPUT_ELEMENT_DESC> inputElemDesc =
	{
		{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"Normal", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u}
	};

	InputLayout* pInputLayout = new InputLayout(gfx, pVertexShaderBlob, inputElemDesc, L"PositionAndColor");
	AddBind(pInputLayout);


	PixelShader* pPixelShader = new PixelShader(gfx, L"Shaders\\Pixel\\LightPS.cso");
	AddBind(pPixelShader);

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, mesh.indices, L"GeoSphereIndexBuffer");
	AddIndexBuffer(pIndexBuffer);

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(pTopology);

	VertexConstantBuffer<CBPerObject>* pVCBPerObject =
		new VertexConstantBuffer<CBPerObject>(gfx, constMatrices, 0u, 1u);
	pCopyVCBMatricesGeoSphere = pVCBPerObject->GetVertexConstantBuffer(); //for updating every frame
	AddBind(pVCBPerObject);

	PixelShaderConstantBuffer<CBPerFrame>* pPSCBPerFrame =
		new PixelShaderConstantBuffer<CBPerFrame>(gfx, constLights, 1u, 1u);
	pCopyPCBLightsGeoSphere = pPSCBPerFrame->GetPixelShaderConstantBuffer();
	AddBind(pPSCBPerFrame);
}

DirectX::XMMATRIX GeoSphere::GetTransform() const noexcept
{
	return m_Matrix * m_Centered;
}

void GeoSphere::Update(float dt) noexcept
{
	alpha = dt;
}

void GeoSphere::UpdateVertexConstantBuffer(Graphics& gfx)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBMatricesGeoSphere, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CBPerObject* object = reinterpret_cast<CBPerObject*>(mappedData.pData);
	object->gWorld = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
	object->gWorldInvTranspose = MathHelper::InverseTranspose(object->gWorld);
	object->gWorldViewProj = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
	gfx.pgfx_pDeviceContext->Unmap(pCopyVCBMatricesGeoSphere, 0u);

	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPCBLightsGeoSphere, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CBPerFrame* frame = reinterpret_cast<CBPerFrame*> (mappedData.pData);

	if (GetAsyncKeyState('0') & 0x8000)
		frame->numLights = 0;
	if (GetAsyncKeyState('1') & 0x8000)
		frame->numLights = 1;

	if (GetAsyncKeyState('2') & 0x8000)
		frame->numLights = 2;

	if (GetAsyncKeyState('3') & 0x8000)
		frame->numLights = 3;
	gfx.pgfx_pDeviceContext->Unmap(pCopyPCBLightsGeoSphere, 0u);

}

