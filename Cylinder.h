#pragma once
#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"
class Cylinder : public Shape
{
public:
	Cylinder(Graphics& gfx, float in_x);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	DirectX::XMMATRIX Update(float dt, DirectX::XMMATRIX in_matrix) noexcept override;
private:
	float alpha{};
	GeometryGenerator::MeshData mesh;
	GeometryGenerator cylinderParts;
};

