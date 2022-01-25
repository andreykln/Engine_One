#include "Cylinder.h"
Cylinder::Cylinder(Graphics& gfx,
	float bottom_radius, float top_radius, float height, UINT slice_count, UINT stack_count)
{
	cylinderParts.CreateCylinder(bottom_radius, top_radius, height, slice_count, stack_count, mesh);

	std::vector<vbPosNormalTexTangent> vertices(mesh.vertices.size());

	for (UINT i = 0; i < mesh.vertices.size(); i++)
	{
		DirectX::XMFLOAT3 p = mesh.vertices[i].position;
		DirectX::XMFLOAT3 n = mesh.vertices[i].normal;
		DirectX::XMFLOAT2 t = mesh.vertices[i].TexC;
		DirectX::XMFLOAT3 tg = mesh.vertices[i].tangentU;

		vertices[i].pos = p;
		vertices[i].normal = n;
		vertices[i].tex = t;
		vertices[i].tangent = tg;
	}
	//build world matrices for 10 columns
	for (int i = 0; i < 5; ++i)
	{
		DirectX::XMStoreFloat4x4(&sCylWorld[i * 2 + 0],
			DirectX::XMMatrixTranslation(-5.0f, 1.5f, -10.0f + i * 5.0f));

		DirectX::XMStoreFloat4x4(&sCylWorld[i * 2 + 1],
			DirectX::XMMatrixTranslation(5.0f, 1.5f, -10.0f + i * 5.0f));
	}
	for (size_t i = 0; i < 10; i++)
	{
		m_CylWorld[i] = DirectX::XMLoadFloat4x4(&sCylWorld[i]);
	}
	pVertexBuffer = gfx.CreateVertexBuffer(vertices, false, false, L"Cylinder vertices");
	pIndexBuffer = gfx.CreateIndexBuffer(mesh.indices, L"Cylinder index buffer");
	indexCount = static_cast<UINT>(mesh.indices.size());

	cylinderMaterial.diffuseAlbedo = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	cylinderMaterial.fresnelR0 = DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f);
	cylinderMaterial.shininess = 0.7f;

}

ID3D11Buffer** Cylinder::GetVertexBuffer()
{
	return &pVertexBuffer;
}


ID3D11Buffer* Cylinder::GetIndexBuffer()
{
	return pIndexBuffer;
}

UINT Cylinder::GetIndexCount()
{
	return indexCount;
}

