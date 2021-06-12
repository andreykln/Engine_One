#include "Picking.h"

Picking::Picking(Graphics& gfx)
{

	UINT vertices = 0;
	UINT triangles = 0;


	std::fstream file(L"models\\car.txt");
	std::string ignore;
	file >> ignore >> vertices;
	file >> ignore >> triangles;
	file >> ignore >> ignore >> ignore >> ignore;

	std::vector<Vertices> verticesFromTXT(vertices);
	verticesCopy.resize(vertices);
	for (size_t i = 0; i < vertices; i++)
	{
		file >> verticesFromTXT[i].position.x >> verticesFromTXT[i].position.y >> verticesFromTXT[i].position.z >>
			verticesFromTXT[i].normal.x >> verticesFromTXT[i].normal.y >> verticesFromTXT[i].normal.z;

		DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&verticesFromTXT[i].position);
		vMin = DirectX::XMVectorMin(vMin, P);
		vMax = DirectX::XMVectorMax(vMax, P);

	}
	using namespace DirectX;
	//fill bounding box
	DirectX::XMStoreFloat3(&carBox.Center, 0.5f * (vMin + vMax));
	DirectX::XMStoreFloat3(&carBox.Extents, 0.5f * (vMax - vMin));

	file >> ignore >> ignore >> ignore;
	UINT indexCount = 3 * triangles;
	std::vector<UINT> indices(indexCount);
	indicesCopy.resize(indexCount);
	for (size_t i = 0; i < triangles; ++i)
	{
		file >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}
	file.close();
	verticesCopy = verticesFromTXT;
	VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, verticesFromTXT, L"TXT");
	AddBind(pVertexBuffer);




	carPSBuffer.mat.ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	carPSBuffer.mat.diffuse = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	carPSBuffer.mat.specular = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 32.0f);

	carPSBuffer.dirLight[0].ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	carPSBuffer.dirLight[0].diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	carPSBuffer.dirLight[0].direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	carPSBuffer.dirLight[0].specular = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	carPSBuffer.dirLight[1].ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	carPSBuffer.dirLight[1].diffuse = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	carPSBuffer.dirLight[1].direction = DirectX::XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);
	carPSBuffer.dirLight[1].specular = DirectX::XMFLOAT4(0.55f, 0.55f, 0.55f, 1.0f);
	carPSBuffer.dirLight[2].ambient = DirectX::XMFLOAT4(0.5, 0.5f, 0.5f, 1.0f);
	carPSBuffer.dirLight[2].diffuse = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	carPSBuffer.dirLight[2].direction = DirectX::XMFLOAT3(0.0f, -0.707f, -0.707f);
	carPSBuffer.dirLight[2].specular = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);


	indicesCopy = indices;
	IndexBuffer* pIndexBuffer = new IndexBuffer(gfx, indices, L"TXTIndexBuffer");
	AddIndexBuffer(pIndexBuffer);

	Topology* pTopology = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	AddBind(pTopology);

	VertexConstantBuffer<CB_VS_Transform>* pVCBPerObject =
		new VertexConstantBuffer<CB_VS_Transform>(gfx, transformMatrices, 0u, 1u);
	pCopyVCBMatricesCar = pVCBPerObject->GetVertexConstantBuffer(); 
	AddBind(pVCBPerObject);

	PixelShaderConstantBuffer<CB_PS_DirectionalL_Fog>* pCarM =
		new PixelShaderConstantBuffer<CB_PS_DirectionalL_Fog>(gfx, carPSBuffer, 0u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyCarMaterial = pCarM->GetPixelShaderConstantBuffer();
	AddBind(pCarM);

	PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>* pLightsCB =
		new PixelShaderConstantBuffer<CB_PS_PerFrameUpdate>(gfx, pscBuffer, 1u, 1u, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	pCopyPCBLightsPerFrame = pLightsCB->GetPixelShaderConstantBuffer();
	AddBind(pLightsCB);

}

void Picking::UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyVCBMatricesCar, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedData));
	CB_VS_Transform* pMatrices = reinterpret_cast<CB_VS_Transform*>(mappedData.pData);
	pMatrices->world = in_world;
	pMatrices->worldInvTranspose = MathHelper::InverseTranspose(in_world);
	pMatrices->worldViewProjection = DirectX::XMMatrixTranspose(in_world * in_ViewProj);
	pMatrices->texTransform = DirectX::XMMatrixIdentity();
	gfx.pgfx_pDeviceContext->Unmap(pCopyVCBMatricesCar, 0u);
}

void Picking::UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPosition)
{


	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyPCBLightsPerFrame, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CB_PS_PerFrameUpdate* frame = reinterpret_cast<CB_PS_PerFrameUpdate*> (mappedData.pData);
	frame->cameraPositon = camPosition;
	if (GetAsyncKeyState('0') & 0x8000)
		frame->numberOfLights = 0;
	if (GetAsyncKeyState('1') & 0x8000)
		frame->numberOfLights = 1;
	if (GetAsyncKeyState('2') & 0x8000)
		frame->numberOfLights = 2;
	if (GetAsyncKeyState('3') & 0x8000)
		frame->numberOfLights = 3;
	gfx.pgfx_pDeviceContext->Unmap(pCopyPCBLightsPerFrame, 0u);


	//back to default
	carPSBuffer.mat.ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	carPSBuffer.mat.diffuse = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	carPSBuffer.mat.specular = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 32.0f);
	
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyCarMaterial, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CB_PS_DirectionalL_Fog* frame0 = reinterpret_cast<CB_PS_DirectionalL_Fog*> (mappedData.pData);
	frame0->mat = carPSBuffer.mat;
	gfx.pgfx_pDeviceContext->Unmap(pCopyCarMaterial, 0u);


}

void Picking::Pick(DirectX::XMMATRIX view, DirectX::XMMATRIX projection, int mX, int mY)
{
	DirectX::XMFLOAT4X4 P;
	DirectX::XMStoreFloat4x4(&P, projection);
	using namespace DirectX;
	//compute picking ray in view space
	float vx = (2.0f * mX / resolution_width - 1.0f) / P(0, 0);
	float vy = (-2.0f * mY / resolution_height + 1.0f) / P(1, 1);

	//ray definition in view space
	DirectX::XMVECTOR rayOrigin = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR rayDirection = DirectX::XMVectorSet(vx, vy, 0.0f, 1.0f);

	//Transform ray to local space of mesh
	DirectX::XMMATRIX V = view;
	DirectX::XMMATRIX invView = DirectX::XMMatrixInverse(&DirectX::XMMatrixDeterminant(V), V);

	//mesh is only at the center anyway
	DirectX::XMMATRIX invWorld = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX toLocal = DirectX::XMMatrixMultiply(invView, invWorld);

	rayOrigin = DirectX::XMVector3TransformCoord(rayOrigin, toLocal);
	rayDirection = DirectX::XMVector3TransformNormal(rayDirection, toLocal);

	rayDirection = DirectX::XMVector3Normalize(rayDirection);

	//assume not picked anything, init to -1;
	pickedTriangle = -1;
	float tmin = 0.0f;

	//inline bool XM_CALLCONV BoundingBox::Intersects( FXMVECTOR Origin, FXMVECTOR Direction, float& Dist ) const
	if (carBox.Intersects(rayOrigin, rayDirection, tmin))
	{
		tmin = FLT_MAX;
		for (UINT i = 0; i < indicesCopy.size() / 3; ++i)
		{
			//indices for this triangle
			UINT i0 = indicesCopy[i * 3];
			UINT i1 = indicesCopy[i * 3 + 1];
			UINT i2 = indicesCopy[i * 3 + 2];

			//vertices for this triangle
			DirectX::XMVECTOR v0 = DirectX::XMLoadFloat3(&verticesCopy[i0].position);
			DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(&verticesCopy[i1].position);
			DirectX::XMVECTOR v2 = DirectX::XMLoadFloat3(&verticesCopy[i2].position);

			//iterate over all the triangles in order to find the nearest intersection.
			float t = 0.0f;
			if (DirectX::TriangleTests::Intersects(rayOrigin, rayDirection, v0, v1, v2, t))
			{
				if (t < tmin)
				{
					//this is the new nearest picked triangle
					tmin = t;
					pickedTriangle = i;
				}

			}
		}
	}
}

void Picking::SetPickedMaterial(Graphics& gfx)
{

	carPSBuffer.mat.ambient = DirectX::XMFLOAT4(0.9f, 0.9f, 0.0f, 1.0f);
	carPSBuffer.mat.diffuse = DirectX::XMFLOAT4(0.9f, 0.9f, 0.0f, 1.0f);
	carPSBuffer.mat.specular = DirectX::XMFLOAT4(0.9f, 0.9f, 0.0f, 32.0f);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	DX::ThrowIfFailed(gfx.pgfx_pDeviceContext->Map(pCopyCarMaterial, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &mappedData));
	CB_PS_DirectionalL_Fog* frame0 = reinterpret_cast<CB_PS_DirectionalL_Fog*> (mappedData.pData);
	frame0->mat = carPSBuffer.mat;
	gfx.pgfx_pDeviceContext->Unmap(pCopyCarMaterial, 0u);

}

int Picking::GetPickedTriangleIndex()
{
	return pickedTriangle;
}


