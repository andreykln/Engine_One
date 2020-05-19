#pragma once
#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"

class BoxAndCylinder : public Shape
{
public:
	BoxAndCylinder(Graphics& gfx);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	void UpdateVertexConstantBuffer(Graphics& gfx) override;
	UINT GetBoxNumIndices() noexcept;
	UINT GetCylinderNumIndices() noexcept;
	UINT GetBoxNumVertices() noexcept;
private:
	struct Vertex_BandC
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
	};
	GeometryGenerator::MeshData meshBox;
	GeometryGenerator::MeshData meshCylinder;
	GeometryGenerator box;
	GeometryGenerator cylinder;
	ID3D11Buffer* pCopyVertexConstantBuffer2 = nullptr;

};

