#pragma once
#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"
class Pyramid : public Shape
{
public:
	Pyramid(Graphics& gfx, float bottom_side, float height);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	void UpdateVertexConstantBuffer(Graphics& gfx) override;
private :
	GeometryGenerator::MeshData mesh;
	GeometryGenerator pyramid;
	ID3D11Buffer* pCopyMatrixConstBuffer = nullptr;
	ID3D11Buffer* pCopyTimeConstBuffer = nullptr;

	struct ConstantBMatrix
	{
		DirectX::XMMATRIX matrix;
	};
};

