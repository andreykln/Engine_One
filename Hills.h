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
	DirectX::XMFLOAT3 GetHillNormal(float x, float z) const;
	void SetVerticesWidth(UINT in_vertWidth) noexcept;
	void SetVerticesDepth(UINT in_vertDepth) noexcept;
	void UpdateVSMatrices(Graphics& gfx,const DirectX::XMMATRIX& in_world,
		const DirectX::XMMATRIX& in_ViewProj, const DirectX::XMFLOAT3 in_camera);
	void UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPositon);
	void UpdatePSAllLights(Graphics& gfx, DirectX::XMFLOAT3 camPosition, DirectX::XMFLOAT3 camDirection,float totalTime);
	DirectX::XMMATRIX GetHillsOffset() const;

private:
	CB_VS_Transform transformMatrices;
	CB_VS_TransformWithCameraPosition transformMatricesWithCameraPos;
	CB_PS_DirectionalL_Fog directionalLight;
	CB_PS_Dir_Point_Spot_Fog_Lights allLight;
	CB_PS_PerFrameUpdate pscBuffer;
	CB_CameraPosition_ViewProj dsBufferCameraPosition;
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
	ID3D11Buffer* pCopyAllLights = nullptr;
	ID3D11Buffer* pCopyDomainShaderBuffer = nullptr;

	ID3DBlob* pVertexShaderBlob = nullptr;
	GeometryGenerator::MeshData grid;
	GeometryGenerator landscapeGenerated;
	DirectionalLight dirLight;
// 	Material landMat;
	std::vector<TreePointSprite> treesPositions;

};

