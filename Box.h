#pragma once

#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"
class Box : public Shape
{
public:
	Box(Graphics& gfx, float width, float height, float depth);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	float GetAlpha();
private:
	DirectX::XMMATRIX m_Centered = DirectX::XMMatrixTranslation(0.0f, 0.0f, 4.0f);
	DirectX::XMMATRIX m_Matrix = DirectX::XMMatrixIdentity();
	float alpha{};
	GeometryGenerator::MeshData mesh;
	GeometryGenerator box;

};

