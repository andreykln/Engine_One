#pragma once

#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"
#include "directxpackedvector.h" //for XMCOLOR
class Box : public Shape
{
public:
	Box(Graphics& gfx, float width, float height, float depth);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
private:
	struct Vertex_B
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 tangent;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 tex0;
		DirectX::XMFLOAT2 tex1;
		DirectX::PackedVector::XMCOLOR color;
	};
	Vertex_B test;
	void ColorBoxWithRainbow(std::vector<Vertex_B>& vertices) noexcept;
	GeometryGenerator::MeshData mesh;
	GeometryGenerator box;

public:
	float alpha{};
};

