#pragma once

#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"
class Box : public Shape
{
public:
	Box(Graphics& gfx, float width, float height, float depth);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
private:
	GeometryGenerator::MeshData mesh;
	GeometryGenerator box;
public:
	float alpha{};
};

