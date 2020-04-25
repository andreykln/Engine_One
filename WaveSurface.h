#pragma once
#include "Shape.h"
#include "Waves.h"
#include "BindableBase.h"
#include "MathHelper.h"


class WaveSurface : public Shape
{
public:
	WaveSurface(Graphics& gfx, float radius, UINT numSubdivisions);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	void UpdateScene(float totalTime, float dt, Graphics& gfx);
public:
	float alpha{};
private:
	Waves wave;
};

