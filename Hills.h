#pragma once
#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"
class Hills : public Shape
{
public:
	Hills(Graphics& gfx, const float& in_width, const float& in_depth, const UINT& in_m, const UINT& in_n);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	void SetWidth(float in_width) noexcept;
	void SetDepth(float in_depth) noexcept;
	void SetVerticesWidth(UINT in_vertWidth) noexcept;
	void SetVerticesDepth(UINT in_vertDepth) noexcept;
private:
	float width{};
	float depth{};
	UINT m{};
	UINT n{};
	float GetHeight(float x, float z) const;
	float alpha{};
	GeometryGenerator::MeshData grid;
	GeometryGenerator landscapeGenerated;
	DirectX::XMMATRIX m_Centered = DirectX::XMMatrixTranslation(0.0f, 0.0f, 4.0f);
	DirectX::XMMATRIX m_Matrix = DirectX::XMMatrixTranslation(0.0f, -35.0f, 90.0f) *
		DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(-30));


};

