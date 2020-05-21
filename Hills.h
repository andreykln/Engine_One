#pragma once
#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"
#include "LightHelper.h"
#include "MathHelper.h"
class Hills : public Shape
{
public:
	Hills(Graphics& gfx, float in_width, float in_depth, UINT in_m, UINT in_n, bool isFlat);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	void SetWidth(float in_width) noexcept;
	void SetDepth(float in_depth) noexcept;
	DirectX::XMFLOAT3 GetHillNormal(float x, float z) const;
	void SetVerticesWidth(UINT in_vertWidth) noexcept;
	void SetVerticesDepth(UINT in_vertDepth) noexcept;
	void UpdateVertexConstantBuffer(Graphics& gfx) override;
	float GetAlpha() const noexcept;
private:
	bool flatSurface = false;
	float width{};
	float depth{};
	UINT m{};
	UINT n{};
	float GetHeight(float x, float z) const;
	GeometryGenerator::MeshData grid;
	GeometryGenerator landscapeGenerated;
	DirectionalLight dirLight;
	PointLight pointLight;
	SpotLight spotLight;
	Material landMat;
	Material wavesMat;
	//fir sin terrain
// 	DirectX::XMMATRIX m_Matrix = DirectX::XMMatrixTranslation(0.0f, -35.0f, 90.0f) *
// 		DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(-30)); //for terrain


public:
//	float alpha{};

};

