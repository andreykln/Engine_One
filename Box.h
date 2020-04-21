#pragma once

#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"
class Box : public Shape
{
public:
	Box(Graphics& gfx, float width, float height, float depth);
	DirectX::XMMATRIX GetTransform() const noexcept override;
// 	DirectX::XMMATRIX Update(float dt, DirectX::XMMATRIX in_matrix) noexcept override;
	float GetAlpha();
private:
	DirectX::XMMATRIX m_Matrix = DirectX::XMMatrixIdentity();
	float alpha{};
	GeometryGenerator::MeshData mesh;
	GeometryGenerator box;
	DirectX::XMMATRIX m_Translation = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_Rotation = DirectX::XMMatrixIdentity();

};

