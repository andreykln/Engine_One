#pragma once
#include "Shape.h"
#include "LightHelper.h"
#include "MathHelper.h"
class Hills : public Shape
{
public:
	Hills(Graphics& gfx, float in_width, float in_depth, UINT in_m, UINT in_n);
	ID3D11Buffer** GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	UINT GetIndexCount();

	MaterialEx plateMaterial;
	const std::wstring diffuseMap = L"floor";
	const std::wstring normalMap = L"floor_nmap";
	const DirectX::XMMATRIX plateScaling = DirectX::XMMatrixScaling(10.0f, 10.0f, 10.0f);
private:
	float width{};
	float depth{};
	UINT m{};
	UINT n{};

	GeometryGenerator::MeshData grid;
	GeometryGenerator landscapeGenerated;

	ID3D11Buffer* pVertexBuffer = nullptr;
	ID3D11Buffer* pIndexBuffer = nullptr;
	UINT indexCount = 0;
};

