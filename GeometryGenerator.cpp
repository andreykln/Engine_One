#include "GeometryGenerator.h"


void GeometryGenerator::CreateGrid(float width, float depth, UINT m, UINT n, MeshData& meshData)
{
	meshData.vertices.clear();
	meshData.indices.clear();
	UINT vertexCount = m * n;
	UINT triangleCount = (m - 1) * (n - 1) * 2;
	float halfWidth = 0.5f * width;
	float halfDepth = 0.5f * depth;
	float dx = width / (n - 1);
	float dz = depth / (m - 1);
	meshData.vertices.resize(vertexCount);
	for (UINT i = 0; i < m; ++i)
	{
		float z = halfDepth - i * dz;
		for (UINT j = 0; j < n; ++j)
		{
			float x = -halfWidth + j * dx;
			meshData.vertices[(long long)i * n + (long long)j].position = DirectX::XMFLOAT3(x, 0.0f, z);
		}
	}

	meshData.indices.resize((long long)triangleCount * 3);
	UINT k = 0;
	for (UINT i = 0; i < m - 1; ++i)
	{
		for (UINT j = 0; j < n - 1; ++j)
		{
			meshData.indices[k] = i * n + j;
			meshData.indices[(long long)k + 1] = i * n + j + 1;
			meshData.indices[(long long)k + 2] = (i + 1) * n + j;
					 
			meshData.indices[(long long)k + 3] = (i + 1) * n + j;
			meshData.indices[(long long)k + 4] = i * n + j + 1;
			meshData.indices[(long long)k + 5] = (i + 1) * n + j + 1;
			k += 6; // next quad
		}
	}
}

void GeometryGenerator::CreateCylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData& meshData)
{
	meshData.vertices.clear();
	meshData.indices.clear();

	//stacks
	float stackHeight = height / stackCount;
	//Amount to increment radius as we move up each stack
	float radiusStep = (topRadius - bottomRadius) / stackCount;
	UINT ringCount = stackCount + 1;
	//compute vertices for each stack ring starting at the bottom and moving up
	for (UINT i = 0; i < ringCount; ++i)
	{
		float y = -0.5f * height + float(i) * stackHeight; //height of i'th ring
		float r = bottomRadius + float(i) * radiusStep; //radius of i'th ring

		//vertices of ring
		float dTheta = 2.0f * DirectX::XM_PI / sliceCount;
		for (UINT j = 0; j <= sliceCount ; ++j)
		{
			Vertex vertex;
			float c = cosf(j * dTheta);
			float s = sinf(j * dTheta);
			vertex.position = DirectX::XMFLOAT3(r * c, y, r * s);
			vertex.TexC.x = (float)j / sliceCount;
			vertex.TexC.y = 1.0f - (float)i / stackCount;
			// Cylinder can be parameterized as follows, where we
			// introduce v parameter that goes in the same direction
			// as the v tex-coord so that the bitangent goes in the
			// same direction as the v tex-coord.
			// Let r0 be the bottom radius and let r1 be the
			// top radius.
			// y(v) = h - hv for v in [0,1].
			// r(v) = r1 + (r0-r1)v
			//
			// x(t, v) = r(v)*cos(t)
			// y(t, v) = h - hv
			// z(t, v) = r(v)*sin(t)
			//
			// dx/dt = -r(v)*sin(t)
			// dy/dt = 0
			// dz/dt = +r(v)*cos(t)
			//
			// dx/dv = (r0-r1)*cos(t)
			// dy/dv = -h
			// dz/dv = (r0-r1)*sin(t)
			// TangentU us unit length

			vertex.tangentU = DirectX::XMFLOAT3(-s, 0.0f, c);
			float dr = bottomRadius - topRadius;

			DirectX::XMFLOAT3 bitangent(dr * c, -height, dr * s);
			DirectX::XMVECTOR T = DirectX::XMLoadFloat3(&vertex.tangentU);
			DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&bitangent);
			DirectX::XMVECTOR N = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(T, B));
			DirectX::XMStoreFloat3(&vertex.normal, N);
			meshData.vertices.push_back(vertex);

			// Indices
			// Add one because we duplicate the first and last vertex per ring
			// since the texture coordinates are different.
			UINT ringVertexCount = sliceCount + 1;
			for (UINT i = 0; i < stackCount; ++i)
			{
				for (UINT j = 0; j < sliceCount; ++j)
				{
					meshData.indices.push_back(i * ringVertexCount + j);
					meshData.indices.push_back((i + 1) * ringVertexCount + j);
					meshData.indices.push_back((i + 1) * ringVertexCount + j + 1);
					meshData.indices.push_back(i * ringVertexCount + j);
					meshData.indices.push_back((i + 1) * ringVertexCount + j + 1);
					meshData.indices.push_back(i * ringVertexCount + j + 1);
				}
			}
		}
	}
	BuildCylinderTopCap(bottomRadius, topRadius,
		height, sliceCount, stackCount, meshData);
	BuildCylinderBottomCap(bottomRadius, topRadius,
		height, sliceCount, stackCount, meshData);
}

void GeometryGenerator::BuildCylinderTopCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData& meshData)
{
	UINT baseIndex = (UINT)meshData.vertices.size();
	float y = 0.5f * height;
	//vertices of the ring
	float dTheta = 2.0f * DirectX::XM_PI / sliceCount;
	// Duplicate cap ring vertices because the texture coordinates
	// and normals differ.
	for (UINT i = 0; i <= sliceCount; ++i)
	{
		float x = topRadius * cosf(i * dTheta);
		float z = topRadius * sinf(i * dTheta);
		// Scale down by the height to try and make top cap
		// texture coord area proportional to base.
		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

		meshData.vertices.push_back(Vertex(x,y,z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));
	}
	//Cap center vertex
	meshData.vertices.push_back(
		Vertex(0.0f, y, 0.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			0.5f, 0.5f));

	// Index of center vertex.
	UINT centerIndex = (UINT)meshData.vertices.size() - 1;
	for (UINT i = 0; i < sliceCount; ++i)
	{
		meshData.indices.push_back(centerIndex);
		meshData.indices.push_back(baseIndex + i + 1);
		meshData.indices.push_back(baseIndex + i);
	}
}

void GeometryGenerator::BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData& meshData)
{
	UINT baseIndex = (UINT)meshData.vertices.size();
	float y = -0.5f * height;
	//vertices of the ring
	float dTheta = 2.0f * DirectX::XM_PI / sliceCount;
	// Duplicate cap ring vertices because the texture coordinates
	// and normals differ.
	for (UINT i = 0; i <= sliceCount; ++i)
	{
		float x = bottomRadius * cosf(i * dTheta);
		float z = bottomRadius * sinf(i * dTheta);
		// Scale down by the height to try and make top cap
		// texture coord area proportional to base.
		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

		meshData.vertices.push_back(Vertex(
				x, y, z,
				0.0f, -1.0f,
				0.0f, 1.0f,
				0.0f, 0.0f,
				u, v));
	}
	//Cap center vertex
	meshData.vertices.push_back(
		Vertex(0.0f, y, 0.0f,
			0.0f, -1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			0.5f, 0.5f));

	// Index of center vertex.
	UINT centerIndex = (UINT)meshData.vertices.size() - 1;
	for (UINT i = 0; i < sliceCount; ++i)
	{
		meshData.indices.push_back(centerIndex);
		meshData.indices.push_back(baseIndex + i);
		meshData.indices.push_back(baseIndex + i + 1);
	}
}

