#pragma once
#include "BindableBase.h"
#include "Shape.h"

class Cube : public Shape
{
public:
	Cube(Graphics& gfx, float& in_x, float& in_y, float& in_z);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;

private:
	//rotation
	float alpha = 0.0f;

	//translation
	float x = 0.0f;
	float y = 0.0f;
	float z = 10.0f;

};

