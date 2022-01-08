#include "Sky.h"

Sky::Sky(Graphics& gfx)
{
	geoGen.CreateSphere(5000.0f, 30, 30, mesh);

	std::vector<DirectX::XMFLOAT3> vertices(mesh.vertices.size());

	for (size_t i = 0; i < mesh.vertices.size(); i++)
	{
		vertices[i] = mesh.vertices[i].position;
	}

	pVertexBuffer = gfx.CreateVertexBuffer(vertices, false, false, L"SkyBox vertices");
	pIndexBuffer = gfx.CreateIndexBuffer(mesh.indices, L"SkybBox indices");
	indexCount = mesh.indices.size();
}

ID3D11Buffer** Sky::GetVertexBuffer()
{
	return &pVertexBuffer;
}

ID3D11Buffer* Sky::GetIndexBuffer()
{
	return pIndexBuffer;
}

UINT Sky::GetIndexCount()
{
	return indexCount;
}

