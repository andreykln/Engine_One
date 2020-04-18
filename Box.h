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
	std::vector<DirectX::XMMATRIX> matrixArray;
	DirectX::XMMATRIX m_Matrix = DirectX::XMMatrixIdentity();
	float alpha{};
	GeometryGenerator::MeshData mesh;
	GeometryGenerator box;
	DirectX::XMMATRIX m_Translation = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_Rotation = DirectX::XMMatrixIdentity();

};

