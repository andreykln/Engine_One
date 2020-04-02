#pragma once
#include "BindableBase.h"
#include "Shape.h"

class Cube : public Shape
{
public:
	Cube(Graphics& gfx);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
};

