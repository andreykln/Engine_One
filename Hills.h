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
	void SetEyePosition(DirectX::XMFLOAT3 in_Eye) noexcept;
	float GetAlpha() const noexcept;
private:
	struct PerFrame
	{
		PerFrame() { ZeroMemory(this, sizeof(this)); }
		DirectionalLight gDirLight;
		PointLight gPointLight;
		SpotLight gSpotLight;
		DirectX::XMFLOAT3 gEyePosW;
		float padding;
	};
	PerFrame perFrameConstBuff;
	struct PerObject
	{
		PerObject() { ZeroMemory(this, sizeof(this)); }
		DirectX::XMFLOAT4X4 gWorld;
		DirectX::XMFLOAT4X4 gWorldInvTranspose;
		DirectX::XMFLOAT4X4 gWorldViewProj;
		Material gMaterial;
	};

	PerObject perObjectConstBuff;
	bool flatSurface = false;
	float width{};
	float depth{};
	UINT m{};
	UINT n{};
	float GetHeight(float x, float z) const;
	DirectX::XMFLOAT3 eyePos = {0.0f, 0.0f, 0.0f};
	DirectX::XMVECTOR pos;
	DirectX::XMVECTOR target;

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

