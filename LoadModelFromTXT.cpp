#include "LoadModelFromTXT.h"

LoadModelFromTXT::LoadModelFromTXT(Graphics& gfx, const std::wstring& path)
{
	struct Vertices
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
	};
	UINT vertices = 0;
	UINT triangles = 0;



	constBuffPerFrame.objectMaterial.ambient = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	constBuffPerFrame.objectMaterial.diffuse = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	constBuffPerFrame.objectMaterial.specular = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);

	constBuffPerFrame.dirLight[0].ambient = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	constBuffPerFrame.dirLight[0].diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	constBuffPerFrame.dirLight[0].direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	constBuffPerFrame.dirLight[0].specular = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);

	constBuffPerFrame.dirLight[1].ambient = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	constBuffPerFrame.dirLight[1].diffuse = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	constBuffPerFrame.dirLight[1].direction = DirectX::XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);
	constBuffPerFrame.dirLight[1].specular = DirectX::XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);

	constBuffPerFrame.dirLight[2].ambient = DirectX::XMFLOAT4(0.0, 0.0f, 0.0f, 1.0f);
	constBuffPerFrame.dirLight[2].diffuse = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	constBuffPerFrame.dirLight[2].direction = DirectX::XMFLOAT3(0.0f, -0.707f, -0.707f);
	constBuffPerFrame.dirLight[2].specular = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	std::fstream file(path);
	std::string ignore;
	file >> ignore >> vertices;
	file >> ignore >> triangles;
	file >> ignore >> ignore >> ignore >> ignore;

	std::vector<Vertices> verticesFromTXT(vertices);

	DirectX::XMFLOAT4 black(0.0f, 0.0f, 0.0f, 1.0f);
	for (size_t i = 0; i < vertices; i++)
	{
		file >> verticesFromTXT[i].position.x >> verticesFromTXT[i].position.y >> verticesFromTXT[i].position.z >>
			verticesFromTXT[i].normal.x  >> verticesFromTXT[i].normal.y  >> verticesFromTXT[i].normal.z;
	}
	file >> ignore >> ignore >> ignore;
	UINT indexCount = 3 * triangles;
	std::vector<UINT> indices(indexCount);

	for (size_t i = 0; i < triangles; ++i)
	{
		file >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}
	file.close();


	VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, verticesFromTXT, L"TXT");
	AddBind(pVertexBuffer);

	VertexShader* pVertexShader = new VertexShader(gfx, L"Shaders\\Vertex\\SkullVS.cso");
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


	PixelShader* pPixelShader = new PixelShader(gfx, L"Shaders\\Pixel\\SkullPS.cso");
	AddBind(pPixelShader);

	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, indices, L"TXTIndexBuffer");
	AddIndexBuffer(pIndexBuffer);

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(pTopology);

	VertexConstantBuffer<CBPerObject>* pVCBPerObject =
		new VertexConstantBuffer<CBPerObject>(gfx, constBuffPerObject, 0u, 1u);
	pCopyVCBPerObject = pVCBPerObject->GetVertexConstantBuffer(); //for updating every frame
	AddBind(pVCBPerObject);

	PixelShaderConstantBuffer<CBPerFrame>* pPSCBPerFrame =
		new PixelShaderConstantBuffer<CBPerFrame>(gfx, constBuffPerFrame, 1u, 1u);
	pCopyPCBPerFrame = pPSCBPerFrame->GetPixelShaderConstantBuffer();
	AddBind(pPSCBPerFrame);

}

DirectX::XMMATRIX LoadModelFromTXT::GetTransform() const noexcept
{
	return m_Matrix * m_Centered;
}

void LoadModelFromTXT::Update(float dt) noexcept
{
	alpha = dt;
}

void LoadModelFromTXT::UpdateVertexConstantBuffer(Graphics& gfx)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBPerObject, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CBPerObject* object = reinterpret_cast<CBPerObject*>(mappedData.pData);
	object->gWorld = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
	object->gWorldInvTranspose = MathHelper::InverseTranspose(object->gWorld);
	object->gWorldViewProj = DirectX::XMMatrixTranspose(GetTransform() * gfx.GetProjection());
	gfx.pgfx_pDeviceContext->Unmap(pCopyVCBPerObject, 0u);

	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPCBPerFrame, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CBPerFrame* frame = reinterpret_cast<CBPerFrame*> (mappedData.pData);

	if (GetAsyncKeyState('0') & 0x8000)
		frame->numLights = 0;
	if (GetAsyncKeyState('1') & 0x8000)
		frame->numLights = 1;

	if (GetAsyncKeyState('2') & 0x8000)
		frame->numLights = 2;

	if (GetAsyncKeyState('3') & 0x8000)
		frame->numLights = 3;
	gfx.pgfx_pDeviceContext->Unmap(pCopyPCBPerFrame, 0u);

}

void LoadModelFromTXT::SetCameraMatrix(DirectX::XMMATRIX in_matrix) noexcept
{
	m_Matrix = in_matrix;
}
