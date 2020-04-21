#pragma once
#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"

class GeoSphere : public Shape
{
public:
	GeoSphere(Graphics& gfx, float in_x);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept;
	void SetMatrix(DirectX::XMMATRIX in_matrix) noexcept;
private:
	DirectX::XMMATRIX m_Matrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_Centered = DirectX::XMMatrixTranslation(0.0f, 0.0f, 4.0f);
public:
	float alpha{};
private:
	GeometryGenerator::MeshData mesh;
	GeometryGenerator sphere;
};

