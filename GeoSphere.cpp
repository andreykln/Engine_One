#include "GeoSphere.h"
#include "App.h"

GeoSphere::GeoSphere(Graphics& gfx, float radius, UINT numSubdivisions)
{
	sphere.CreateGeosphere(radius, numSubdivisions, mesh);
	std::vector<vbPosNormalTexTangent> vertices(mesh.vertices.size());
	for (UINT i = 0; i < mesh.vertices.size(); i++)
	{
		DirectX::XMFLOAT3 p = mesh.vertices[i].position;
		DirectX::XMFLOAT3 n = mesh.vertices[i].normal;
		DirectX::XMFLOAT2 t = mesh.vertices[i].TexC;
		if (std::isnan(t.x))
		{
			t = { 0.5f, 0.0f };
		}
		vertices[i].pos = p;
		vertices[i].normal = mesh.vertices[i].normal;
		vertices[i].tex = t;
		vertices[i].tangent = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	
	}

	//build world matrices for 10 spheres
	for (int i = 0; i < 5; ++i)
	{
		DirectX::XMStoreFloat4x4(&sGeoSphereWorld[i * 2 + 0],
			DirectX::XMMatrixTranslation(-5.0f, 3.5f, -10.0f + i * 5.0f));

		DirectX::XMStoreFloat4x4(&sGeoSphereWorld[i * 2 + 1],
			DirectX::XMMatrixTranslation(5.0f, 3.5f, -10.0f + i * 5.0f));
	}

	for (size_t i = 0; i < 10; i++)
	{
		m_SphereWorld[i] = DirectX::XMLoadFloat4x4(&sGeoSphereWorld[i]);
	}

	pVertexBuffer = gfx.CreateVertexBuffer(vertices, false, false, L"Spheres Vertex buffer");
	pIndexBuffer = gfx.CreateIndexBuffer(mesh.indices, L"Spheres Index Buffer");
	indexCount = mesh.indices.size();


	sphereMaterial.diffuseAlbedo = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	sphereMaterial.fresnelR0 = DirectX::XMFLOAT3(0.98f, 0.97f, 0.95f);
	sphereMaterial.shininess = 0.9f;
}


ID3D11Buffer** GeoSphere::GetVertexBuffer()
{
	return &pVertexBuffer;
}

ID3D11Buffer* GeoSphere::GetIndexBuffer()
{
	return pIndexBuffer;
}

UINT GeoSphere::GetIndexCount()
{
	return indexCount;
}

