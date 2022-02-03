#include "DisplacementWaves.h"

DisplacementWaves::DisplacementWaves(Graphics& gfx)
{
	geoGen.CreateGrid(50.0f, 50.0f, 65, 45, mesh);
	std::vector<vbPosNormalTexTangent> vertices(mesh.vertices.size());

	wavesMaterial.diffuseAlbedo = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 0.7f);
	wavesMaterial.fresnelR0 = DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f);
	wavesMaterial.shininess = 0.7f;

	for (int i = 0; i < mesh.vertices.size(); ++i)
	{
		vertices[i].pos = mesh.vertices[i].position;
		vertices[i].normal = DirectX::XMFLOAT3{ 0.0f, 1.0f, 0.0f };
		vertices[i].tex = mesh.vertices[i].TexC;
		vertices[i].tangent = mesh.vertices[i].tangentU;
	}

	pVertexBuffer = gfx.CreateVertexBuffer(vertices, false, false, L"Tessellation waves VB");
	indexCount = static_cast<UINT>(mesh.indices.size());
	pIndexBuffer = gfx.CreateIndexBuffer(mesh.indices, L"Tessellation waves IB");
}

ID3D11Buffer** DisplacementWaves::GetVertexBuffer()
{
	return &pVertexBuffer;
}

ID3D11Buffer* DisplacementWaves::GetIndexBuffer()
{
	return pIndexBuffer;
}

UINT DisplacementWaves::GetIndexCount()
{
	return indexCount;
}

