#include "Skull.h"

Skull::Skull(Graphics& gfx, const std::wstring& path)
{
	struct Vertices
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
	};
	struct VerticesSM
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT4 color;
	};

	UINT vertices = 0;
	UINT triangles = 0;

	skullMaterial.diffuseAlbedo = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	skullMaterial.fresnelR0 = DirectX::XMFLOAT3(0.6f, 0.6f, 0.6f);
	skullMaterial.shininess = 0.8f;

	std::fstream file(path);
	std::string ignore;
	file >> ignore >> vertices;
	file >> ignore >> triangles;
	file >> ignore >> ignore >> ignore >> ignore;

	std::vector<vbPosNormalTexTangent> verticesFromTXT(vertices);

	for (size_t i = 0; i < vertices; i++)
	{
		using namespace DirectX;
		file >> verticesFromTXT[i].pos.x >> verticesFromTXT[i].pos.y >> verticesFromTXT[i].pos.z >>
			verticesFromTXT[i].normal.x >> verticesFromTXT[i].normal.y >> verticesFromTXT[i].normal.z;
		verticesFromTXT[i].tex = { 0.0f, 0.0f };

		XMVECTOR P = XMLoadFloat3(&verticesFromTXT[i].pos);
		XMVECTOR N = XMLoadFloat3(&verticesFromTXT[i].normal);
		// Generate a tangent vector so normal mapping works.  We aren't applying
		// a texture map to the skull, so we just need any tangent vector so that
		// the math works out to give us the original interpolated vertex normal.
		XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		auto a = XMVector3Dot(N, up);
		auto b = XMVectorGetX(XMVector3Dot(N, up));
		auto c = fabsf(XMVectorGetX(XMVector3Dot(N, up)));

		if (fabsf(XMVectorGetX(XMVector3Dot(N, up))) < 1.0f - 0.001f)
		{
			XMVECTOR T = XMVector3Normalize(XMVector3Cross(up, N));
			DirectX::XMStoreFloat3(&verticesFromTXT[i].tangent, T);
		}
		else
		{
			up = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
			XMVECTOR T = XMVector3Normalize(XMVector3Cross(N, up));
			DirectX::XMStoreFloat3(&verticesFromTXT[i].tangent, T);
		}

	}
	file >> ignore >> ignore >> ignore;


	UINT indexNum = 3 * triangles;
	std::vector<UINT> indices(indexNum);

	for (size_t i = 0; i < triangles; ++i)
	{
		file >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}
	file.close();

	pVertexBuffer = gfx.CreateVertexBuffer(verticesFromTXT, false, false, L"SkullVertices");

	indexCount = static_cast<UINT>(indices.size());
	pIndexBuffer = gfx.CreateIndexBuffer(indices, L"Skull index buffer");
}

ID3D11Buffer**  Skull::GetVertexBuffer()
{
	return &pVertexBuffer;
}

ID3D11Buffer* Skull::GetIndexBuffer()
{
	return pIndexBuffer;
}

UINT Skull::GetIndexCount()
{
	return indexCount;
}
