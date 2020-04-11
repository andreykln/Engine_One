#include "GeometryGenerator.h"


void GeometryGenerator::CreateGrid(float width, float depth, UINT m, UINT n, MeshData& meshData)
{
	UINT vertexCount = m * n;
	UINT triangleCount = (m - 1) * (n - 1) * 2;


	float halfWidth = 0.5f * width;
	float halfDepth = 0.5f * depth;

	float dx = width / (n - 1);
	float dz = depth / (m - 1);

// 	float du = 1.0f / (n - 1);
// 	float dv = 1.0f / (m - 1);
	meshData.Vertices.resize(vertexCount);
	for (UINT i = 0; i < m; ++i)
	{
		float z = halfDepth - i * dz;
		for (UINT j = 0; j < n; ++j)
		{
			float x = -halfWidth + j * dx;
			meshData.Vertices[(long long)i * n + (long long)j].Position = DirectX::XMFLOAT3(x, 0.0f, z);
		}
	}

	meshData.Indices.resize((long long)triangleCount * 3);
	UINT k = 0;
	for (UINT i = 0; i < m - 1; ++i)
	{
		for (UINT j = 0; j < n - 1; ++j)
		{
			meshData.Indices[k] = i * n + j;
			meshData.Indices[(long long)k + 1] = i * n + j + 1;
			meshData.Indices[(long long)k + 2] = (i + 1) * n + j;

			meshData.Indices[(long long)k + 3] = (i + 1) * n + j;
			meshData.Indices[(long long)k + 4] = i * n + j + 1;
			meshData.Indices[(long long)k + 5] = (i + 1) * n + j + 1;
			k += 6; // next quad
		}
	}


}


