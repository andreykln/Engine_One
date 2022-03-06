#include "Hills.h"
#include <cmath>

Hills::Hills(Graphics& gfx, float in_width, float in_depth, UINT in_m, UINT in_n)
	: width(in_width), depth(in_depth), m(in_m), n(in_n)
{
	landscapeGenerated.CreateGrid(width, depth, m, n, grid);
	std::vector<vbPosNormalTexTangent> vertices(grid.vertices.size());

	for (size_t i = 0; i < grid.vertices.size(); ++i)
	{
		DirectX::XMFLOAT3 p = grid.vertices[i].position;
		vertices[i].pos = p;
		vertices[i].normal = DirectX::XMFLOAT3{ 0.0f, 1.0f, 0.0f };
		vertices[i].tex = grid.vertices[i].TexC;
		vertices[i].tangent = grid.vertices[i].tangentU;
	}
	pVertexBuffer = gfx.CreateVertexBuffer(vertices, false, false, L"Plate vertex buffer");
	pIndexBuffer = gfx.CreateIndexBuffer(grid.indices, L"Plate index buffer");
	indexCount = static_cast<UINT>(grid.indices.size());

	plateMaterial.diffuseAlbedo = DirectX::XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	plateMaterial.fresnelR0 = DirectX::XMFLOAT3(0.2f, 0.2f, 0.2f);
	plateMaterial.shininess = 0.9f;
}

ID3D11Buffer** Hills::GetVertexBuffer()
{
	return &pVertexBuffer;
}

ID3D11Buffer* Hills::GetIndexBuffer()
{
	return pIndexBuffer;
}

UINT Hills::GetIndexCount()
{
	return indexCount;
}
