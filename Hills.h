#pragma once
#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"
#include "LightHelper.h"
#include "MathHelper.h"
class Hills : public Shape
{
public:
	Hills(Graphics& gfx, float in_width, float in_depth, UINT in_m, UINT in_n, DemoSwitch demo);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	DirectX::XMFLOAT3 GetHillNormal(float x, float z) const;
	void SetVerticesWidth(UINT in_vertWidth) noexcept;
	void SetVerticesDepth(UINT in_vertDepth) noexcept;
	void UpdateVertexConstantBuffer(Graphics& gfx) override;
	void UpdateConstantBuffers(Graphics& gfx, DirectX::XMFLOAT3& eyePosition, DirectX::XMVECTOR& pos, DirectX::XMVECTOR& target);
	void UpdateVSMatrices(Graphics& gfx,const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj);
	void UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPositon);
	DirectX::XMMATRIX GetHillsOffset() const;
private:

	//TEMPORARY DISABLE IT, DON'T NEED SPOT LIGHTS
// 	struct PerFrame
// 	{
// 		PerFrame() { ZeroMemory(this, sizeof(this)); }
// 		DirectionalLight gDirLight;
// 		PointLight gPointLight;
// 		SpotLight gSpotLight;
// 		DirectX::XMFLOAT3 gEyePosW;
// 		float padding{};
// 		Material gMaterial;
// 	};
	CB_VS_Transform transformMatrices;
	CB_PS_DirectionalL_Fog directionalLight;
	CB_PS_PerFrameUpdate pscBuffer;

	CBPerFrame constLights;
	const DirectX::XMMATRIX grassScaling = DirectX::XMMatrixScaling(5.0f, 5.0f, 5.0f);
	const DirectX::XMMATRIX plateScaling = DirectX::XMMatrixScaling(5.0f, 5.0f, 5.0f);
	const DirectX::XMMATRIX offsetForHillsWithWaves = DirectX::XMMatrixTranslation(0.0f, -4.0f, 0.0f);


	DemoSwitch currentDemo;
	float width{};
	float depth{};
	UINT m{};
	UINT n{};
	float GetHeight(float x, float z) const;

	ID3D11Buffer* pCopyPCBLightsHills = nullptr;
	ID3D11Buffer* pCopyVCBMatricesHills = nullptr;

	ID3DBlob* pVertexShaderBlob = nullptr;
	GeometryGenerator::MeshData grid;
	GeometryGenerator landscapeGenerated;
	DirectionalLight dirLight;
	PointLight pointLight;
	SpotLight spotLight;
	Material landMat;
	Material wavesMat;
	std::vector<TreePointSprite> treesPositions;

};

