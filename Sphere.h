#pragma once
#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"

class Sphere : public Shape
{
public:
	Sphere(Graphics& gfx, float in_x, DirectX::XMMATRIX in_MatrixStartPosition);
	DirectX::XMMATRIX GetTransform() const noexcept override;
// 	DirectX::XMMATRIX Update(float dt, DirectX::XMMATRIX in_matrix) noexcept override;
private:
	float alpha{};
	GeometryGenerator::MeshData mesh;
	GeometryGenerator sphere;
	DirectX::XMMATRIX m_Translation = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_Centered = DirectX::XMMatrixTranslation(0.0f, 0.0f, 4.0f);

};

