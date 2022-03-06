#include "Box.h"
#include "MathHelper.h"


Box::Box(Graphics& gfx, float width, float height, float depth)
{

	box.CreateBox(width, height, depth, mesh);

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

	pVertexBuffer = gfx.CreateVertexBuffer(vertices, false, false, L"Cube vertex buffer");
	pIndexBuffer = gfx.CreateIndexBuffer(mesh.indices, L"Cube index Buffer");
	indexCount = static_cast<UINT>(mesh.indices.size());
	

	boxMaterial.diffuseAlbedo = DirectX::XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	boxMaterial.fresnelR0 = DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f);
	boxMaterial.shininess = 0.7f;
}

ID3D11Buffer** Box::GetVertexBuffer()
{
	return &pVertexBuffer;
}

ID3D11Buffer* Box::GetIndexBuffer()
{
	return pIndexBuffer;
}

UINT Box::GetIndexCount()
{
	return indexCount;
}
