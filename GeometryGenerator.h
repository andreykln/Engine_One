#pragma once
#include <DirectXMath.h>
#include <vector>
#include "d3d11.h"

class GeometryGenerator
{
public:
	struct Vertex
	{
		DirectX::XMFLOAT3 Position = {0.0f, 0.0f, 0.0f};
	};

	struct MeshData
	{
		std::vector<Vertex> Vertices;
		std::vector<UINT> Indices;
	};

	void CreateGrid(float width, float depth, UINT m, UINT n, MeshData& meshData);
};	
	
