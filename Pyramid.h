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
private :
	GeometryGenerator::MeshData mesh;
	GeometryGenerator pyramid;

};

