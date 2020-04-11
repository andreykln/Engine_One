#pragma once
#include <DirectXMath.h>
#include <vector>
#include "d3d11.h"

class GeometryGenerator
{
public:
	struct Vertex
	{
// 		Vertex() {}
// 		Vertex(const DirectX::XMFLOAT3& p,
// 				const DirectX::XMFLOAT3& n,
// 				const DirectX::XMFLOAT3& t,
// 				const DirectX::XMFLOAT2& uv)
// 			: Position(p), Normal(n), TangentU(t), TexC(uv) {}
// 
// 		Vertex(float px, float py, float pz,
// 			float nx, float ny, float nz,
// 			float tx, float ty, float tz,
// 			float u, float v)
// 			:
// 			Position(px, py, pz), Normal(nx, ny, nz), TangentU(tx, ty, tz), TexC(u, v) {}
		DirectX::XMFLOAT3 Position = {0.0f, 0.0f, 0.0f};
// 		DirectX::XMFLOAT3 Normal = { 0.0f, 0.0f, 0.0f };
// 		DirectX::XMFLOAT3 TangentU = { 0.0f, 0.0f, 0.0f };
// 		DirectX::XMFLOAT2 TexC = { 0.0f, 0.0f};
	};

	struct MeshData
	{
		std::vector<Vertex> Vertices;
		std::vector<UINT> Indices;
	};

	void CreateGrid(float width, float depth, UINT m, UINT n, MeshData& meshData);
};	
	
