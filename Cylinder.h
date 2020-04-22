#pragma once
#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"
class Cylinder : public Shape
{
public:
	Cylinder(Graphics& gfx, float in_x);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
private:
	float alpha{};
	GeometryGenerator::MeshData mesh;
	GeometryGenerator cylinderParts;
	DirectX::XMMATRIX m_Centered = DirectX::XMMatrixTranslation(0.0f, 0.0f, 4.0f);
	DirectX::XMMATRIX m_Matrix = DirectX::XMMatrixIdentity();

};

