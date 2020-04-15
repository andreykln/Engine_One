#pragma once
#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"

class Sphere : public Shape
{
public:
	Sphere(Graphics& gfx, float in_x);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
private:
	float alpha{};
	GeometryGenerator::MeshData mesh;
	GeometryGenerator sphere;
};

