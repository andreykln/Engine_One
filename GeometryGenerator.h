#pragma once
#include <DirectXMath.h>
#include <vector>
#include <algorithm>
#include "d3d11.h"

class GeometryGenerator
{
public:
	struct Vertex
	{
		Vertex() {}
		Vertex(const DirectX::XMFLOAT3& p,
			const DirectX::XMFLOAT3& n,
			const DirectX::XMFLOAT3& t,
			const DirectX::XMFLOAT2& uv)
			: position(p), normal(n), tangentU(t), TexC(uv) {}
		Vertex(
			float px, float py, float pz,
			float nx, float ny, float nz,
			float tx, float ty, float tz,
			float u, float v)
			: position(px, py, pz), normal(nx, ny, nz),
			tangentU(tx, ty, tz), TexC(u, v) {}
		DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3 normal = { 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3 tangentU = { 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT2 TexC = { 0.0f, 0.0f };
	};

	struct MeshData
	{
		std::vector<Vertex> vertices;
		std::vector<UINT> indices;
	};

	void CreateGrid(float width, float depth, UINT m, UINT n, MeshData& meshData);
	void CreateBox(float width, float depth, float height, MeshData& meshData);
	void CreatePyramid(float bottom_side, float height, MeshData& meshData);
	void CreateGeosphere(float radius, UINT numSubdivisions, MeshData& meshData);
	void CreateSphere(float radius, UINT sliceCount, UINT stackCount, MeshData& meshData);
	void CreateCylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData& meshData);
	void CreateCylinderNoCaps(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData& meshData);
private:
	void BuildCylinderTopCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData& meshData);
	void BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData& meshData);
	void Subdivide(MeshData& meshData);
	float PolarAngleFromXY(float x, float y);

};	
	

