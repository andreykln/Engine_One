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

void GeometryGenerator::CreateBox(float width, float depth, float height, MeshData& meshData)
{
	//
	// Create the vertices.
	//

	Vertex v[24];

	float w2 = 0.5f * width;
	float h2 = 0.5f * height;
	float d2 = 0.5f * depth;

	// Fill in the front face vertex data.
// 	v[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
// 	v[1] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
// 	v[2] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.7f, 0.0f);
// 	v[3] = Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 2.0f, 3.0f);


	v[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[1] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[3] = Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);


	// Fill in the back face vertex data.
	v[4] = Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[5] = Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[6] = Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[7] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the top face vertex data.
	v[8] = Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[9] = Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[10] = Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[11] = Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	v[12] = Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[13] = Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[14] = Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[15] = Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the left face vertex data.
	v[16] = Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[17] = Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[18] = Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[19] = Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the right face vertex data.
	v[20] = Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[21] = Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[22] = Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[23] = Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	meshData.vertices.assign(&v[0], &v[24]);

	//
	// Create the indices.
	//

	UINT i[36];

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	meshData.indices.assign(&i[0], &i[36]);





	/*meshData.vertices.clear();
	meshData.indices.clear();
	std::vector<DirectX::XMFLOAT3> cubeCoord
	{
		{DirectX::XMFLOAT3(-width, -height, -depth)},
		{DirectX::XMFLOAT3(width, -height, -depth)},
		{DirectX::XMFLOAT3(width, height, -depth)},

		{DirectX::XMFLOAT3(-width, height, -depth)},
		{DirectX::XMFLOAT3(-width, height, depth)},
		{DirectX::XMFLOAT3(width, height, depth)},

		{DirectX::XMFLOAT3(-width, -height, depth)},
		{DirectX::XMFLOAT3(width, -height, depth)}
	};
	meshData.indices = 
	{
		//front
		0,2,1, 0,3,2,
		//back
		7,5,6, 6,5,4,
		//right side
		1,2,7, 2,5,7,
		//left side
		6,3,0, 6,4,3,
		//up
		2,4,5, 3,4,2,
		//down
		1,7,6, 1,6,0
	};
	meshData.vertices.resize(cubeCoord.size());
	for (size_t i = 0; i < cubeCoord.size(); i++)
	{
		meshData.vertices[i].position =  cubeCoord[i];
	}*/
}

void GeometryGenerator::CreatePyramid(float bottom_side, float height, MeshData& meshData)
{
	using namespace DirectX;
	meshData.vertices.clear();
	meshData.indices.clear();
	float halfHeight = height / 2.0f;
	float halfSide = bottom_side / 2;

// 	DirectX::XMFLOAT3 zero_toFirstCoord = {-halfSide, -halfHeight , -halfSide };
// 	DirectX::XMFLOAT3 zero_toOppositeCoord = {halfSide, -halfHeight , halfSide };
// 	DirectX::XMFLOAT3 top = (zero_toFirstCoord.x + zero_toOppositeCoord.x,
// 		zero_toFirstCoord.y + zero_toOppositeCoord.y,
// 		zero_toFirstCoord.z + zero_toOppositeCoord.z);
	std::vector<DirectX::XMFLOAT3> pyramidCoord
	{
		{DirectX::XMFLOAT3(-halfSide, -halfHeight, -halfSide)},
		{DirectX::XMFLOAT3(-halfSide, -halfHeight, +halfSide)},

		{DirectX::XMFLOAT3(halfSide, -halfHeight, -halfSide)},
		{DirectX::XMFLOAT3(halfSide, -halfHeight, +halfSide)},

		{DirectX::XMFLOAT3(0.0f, halfHeight, 0.0f)}, //top
	};
	meshData.indices =
	{
		//bottom
		0,2,1, 2,3,1,
		//front
		0,4,2,
		//right side
		2,4,3,
		//left side
		1,4,0,
		//back
		3,4,1
	};
	meshData.vertices.resize(pyramidCoord.size());
	for (size_t i = 0; i < pyramidCoord.size(); i++)
	{
		meshData.vertices[i].position = pyramidCoord[i];
	}
}

void GeometryGenerator::CreateGeosphere(float radius, UINT numSubdivisions, MeshData& meshData)
{
	using namespace DirectX; //for multiplying XMVector by a scalar

	//Put a cap on the number of subdivisions
	numSubdivisions = DirectX::XMMin(numSubdivisions, 5u);
	//approximate a sphere by tessellating an icosahedron
	const float X = 0.525731f;
	const float Z = 0.850651f;
	DirectX::XMFLOAT3 pos[12] =
	{
		DirectX::XMFLOAT3(-X, 0.0f, Z), DirectX::XMFLOAT3(X, 0.0f, Z),
		DirectX::XMFLOAT3(-X, 0.0f, -Z), DirectX::XMFLOAT3(X, 0.0f, -Z),
		DirectX::XMFLOAT3(0.0f, Z, X), DirectX::XMFLOAT3(0.0f, Z, -X),
		DirectX::XMFLOAT3(0.0f, -Z, X), DirectX::XMFLOAT3(0.0f, -Z, -X),
		DirectX::XMFLOAT3(Z, X, 0.0f), DirectX::XMFLOAT3(-Z, X, 0.0f),
		DirectX::XMFLOAT3(Z, -X, 0.0f), DirectX::XMFLOAT3(-Z, -X, 0.0f)
	};
	DWORD k[60] =
	{
	1,4,0, 4,9,0, 4,5,9, 8,5,4, 1,8,4,
	1,10,8, 10,3,8, 8,3,5, 3,2,5, 3,7,2,
	3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
	10,1,6, 11,0,9, 2,11,9, 5,2,9, 11,2,7
	};

	meshData.vertices.resize(12);
	meshData.indices.resize(60);

	for (size_t i = 0; i < 12; ++i)
	{
		meshData.vertices[i].position = pos[i];
	}
	for (size_t i = 0; i < 60; ++i)
	{
		meshData.indices[i] = k[i];
	}
	for (size_t i = 0; i < numSubdivisions; ++i)
	{
		Subdivide(meshData);
	}
	// Project vertices onto sphere and scale.
	for (size_t i = 0; i < meshData.vertices.size(); ++i)
	{
		// Project onto unit sphere.
		DirectX::XMVECTOR n = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(
			&meshData.vertices[i].position));
		// Project onto sphere.
		DirectX::XMVECTOR p = radius * n;
		DirectX::XMStoreFloat3(&meshData.vertices[i].position, p);
		DirectX::XMStoreFloat3(&meshData.vertices[i].normal, n);
		// Derive texture coordinates from spherical coordinates.
		
		float theta = PolarAngleFromXY(
			meshData.vertices[i].position.x,
			meshData.vertices[i].position.z);
		float phi = acosf(meshData.vertices[i].position.y / radius);
		meshData.vertices[i].TexC.x = theta / DirectX::XM_2PI;
		meshData.vertices[i].TexC.y = phi / DirectX::XM_PI;
		// Partial derivative of P with respect to theta
		meshData.vertices[i].tangentU.x = -radius * sinf(phi) * sinf(theta);
		meshData.vertices[i].tangentU.y = 0.0f;
		meshData.vertices[i].tangentU.z = +radius * sinf(phi) * cosf(theta);
		DirectX::XMVECTOR T = DirectX::XMLoadFloat3(&meshData.vertices[i].tangentU);
		DirectX::XMStoreFloat3(&meshData.vertices[i].tangentU,
			DirectX::XMVector3Normalize(T));
	}
	
}

void GeometryGenerator::CreateSphere(float radius, UINT sliceCount, UINT stackCount, MeshData& meshData)
{
	meshData.vertices.clear();
	meshData.indices.clear();
	// Compute the vertices stating at the top pole and moving down the stacks.
	// Poles: note that there will be texture coordinate distortion as there is
	// not a unique point on the texture map to assign to the pole when mapping
	// a rectangular texture onto a sphere.
	Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	meshData.vertices.push_back(topVertex);

	float phiStep = DirectX::XM_PI / stackCount;
	float thetaStep = 2.0f * DirectX::XM_PI / sliceCount;
	// Compute vertices for each stack ring (do not count the poles as rings).
	for (UINT i = 1; i <= stackCount - 1; ++i)
	{
		float phi = i * phiStep;

		// Vertices of ring.
		for (UINT j = 0; j <= sliceCount; ++j)
		{
			float theta = j * thetaStep;

			Vertex v;

			// spherical to Cartesian
			v.position.x = radius * sinf(phi) * cosf(theta);
			v.position.y = radius * cosf(phi);
			v.position.z = radius * sinf(phi) * sinf(theta);

			// Partial derivative of P with respect to theta
			v.tangentU.x = -radius * sinf(phi) * sinf(theta);
			v.tangentU.y = 0.0f;
			v.tangentU.z = +radius * sinf(phi) * cosf(theta);

			DirectX::XMVECTOR T = DirectX::XMLoadFloat3(&v.tangentU);
			DirectX::XMStoreFloat3(&v.tangentU, DirectX::XMVector3Normalize(T));

			DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&v.position);
			DirectX::XMStoreFloat3(&v.normal, DirectX::XMVector3Normalize(p));

			v.TexC.x = theta / DirectX::XM_2PI;
			v.TexC.y = phi / DirectX::XM_PI;

			meshData.vertices.push_back(v);
		}
	}

	meshData.vertices.push_back(bottomVertex);

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

	for (UINT i = 1; i <= sliceCount; ++i)
	{
		meshData.indices.push_back(0);
		meshData.indices.push_back(i + 1);
		meshData.indices.push_back(i);
	}

	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	UINT baseIndex = 1;
	UINT ringVertexCount = sliceCount + 1;
	for (UINT i = 0; i < stackCount - 2; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			meshData.indices.push_back(baseIndex + i * ringVertexCount + j);
			meshData.indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			meshData.indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			
			meshData.indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			meshData.indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			meshData.indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	UINT southPoleIndex = (UINT)meshData.vertices.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		meshData.indices.push_back(southPoleIndex);
		meshData.indices.push_back(baseIndex + i);
		meshData.indices.push_back(baseIndex + i + 1);
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

void GeometryGenerator::Subdivide(MeshData& meshData)
{
	// Save a copy of the input geometry.
	MeshData inputCopy = meshData;


	meshData.vertices.resize(0);
	meshData.indices.resize(0);

	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2

	UINT numTris = (unsigned long)inputCopy.indices.size() / 3;
	for (UINT i = 0; i < numTris; ++i)
	{
		Vertex v0 = inputCopy.vertices[inputCopy.indices[(long long)i * 3u + 0]];
		Vertex v1 = inputCopy.vertices[inputCopy.indices[(long long)i * 3u + 1]];
		Vertex v2 = inputCopy.vertices[inputCopy.indices[(long long)i * 3u + 2]];

		//
		// Generate the midpoints.
		//

		Vertex m0, m1, m2;

		// For subdivision, we just care about the position component.  We derive the other
		// vertex components in CreateGeosphere.

		m0.position = DirectX::XMFLOAT3(
			0.5f * (v0.position.x + v1.position.x),
			0.5f * (v0.position.y + v1.position.y),
			0.5f * (v0.position.z + v1.position.z));

		m1.position = DirectX::XMFLOAT3(
			0.5f * (v1.position.x + v2.position.x),
			0.5f * (v1.position.y + v2.position.y),
			0.5f * (v1.position.z + v2.position.z));

		m2.position = DirectX::XMFLOAT3(
			0.5f * (v0.position.x + v2.position.x),
			0.5f * (v0.position.y + v2.position.y),
			0.5f * (v0.position.z + v2.position.z));

		//
		// Add new geometry.
		//

		meshData.vertices.push_back(v0); // 0
		meshData.vertices.push_back(v1); // 1
		meshData.vertices.push_back(v2); // 2
		meshData.vertices.push_back(m0); // 3
		meshData.vertices.push_back(m1); // 4
		meshData.vertices.push_back(m2); // 5

		meshData.indices.push_back(i * 6 + 0);
		meshData.indices.push_back(i * 6 + 3);
		meshData.indices.push_back(i * 6 + 5);

		meshData.indices.push_back(i * 6 + 3);
		meshData.indices.push_back(i * 6 + 4);
		meshData.indices.push_back(i * 6 + 5);

		meshData.indices.push_back(i * 6 + 5);
		meshData.indices.push_back(i * 6 + 4);
		meshData.indices.push_back(i * 6 + 2);

		meshData.indices.push_back(i * 6 + 3);
		meshData.indices.push_back(i * 6 + 1);
		meshData.indices.push_back(i * 6 + 4);
	}
}

float GeometryGenerator::PolarAngleFromXY(float x, float y)
{
	float theta = 0.0f;

	// Quadrant I or IV
	if (x >= 0.0f)
	{
		// If x = 0, then atanf(y/x) = +pi/2 if y > 0
		//                atanf(y/x) = -pi/2 if y < 0
		theta = atanf(y / x); // in [-pi/2, +pi/2]

		if (theta < 0.0f)
			theta += 2.0f * DirectX::XM_PI; // in [0, 2*pi).
	}

	// Quadrant II or III
	else
		theta = atanf(y / x) + DirectX::XM_PI; // in [0, 2*pi).

	return theta;
}