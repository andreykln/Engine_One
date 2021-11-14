#pragma once
#include "Shape.h"

class Skull : public Shape
{
public:
	Skull(Graphics& gfx, const std::wstring& path, DemoSwitch in_currentDemo);
	void DrawSkull(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj,
		DirectX::XMFLOAT3 camPosition);

	//updating only light for mirror demo
	void UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj);
	void UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPosition);
	void UpdatePSLightDirection(Graphics& gfx, DirectX::XMFLOAT3 lightDirection, UINT index);
	DirectX::XMMATRIX GetMirroredSkullTranslation() const;
	void SetNewLightDirection(DirectX::XMFLOAT3& lightDirection, UINT index) noexcept;
	void SetNewLightDirection_(DirectX::XMFLOAT3 lightDirection[3], UINT index) noexcept;
	void UpdateEyePosition(DirectX::XMFLOAT3 eyePos) noexcept;
	void UpdateMaterial(Graphics& gfx, bool shadow) noexcept;
	DirectionalLight GetLightDirection(UINT index) const noexcept;


	//sm
	void UpdateShadomMapGenBuffers(Graphics& gfx, const DirectX::XMMATRIX& in_lightWorld, DirectX::XMFLOAT3 newCamPosition);
	void UpdateShadowMapDrawBuffers(Graphics& gfx, DirectX::XMFLOAT3 newCamPosition, const DirectX::XMMATRIX& newShadowTransform,
		const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj, ID3D11ShaderResourceView* pShadowMapSRV,
		DirectX::XMFLOAT3* newLightDirection);

	Material shadowMaterial;
	Material skullMatData;

	CB_VS_Transform transformMatrices;
	CB_PS_DirectionalL_Fog directionalLight;
	CB_PS_PerFrameUpdate pscBuffer;
	CB_PS_Skull_Mirror mirrorBuffer;
	CB_PS_Skull_Mat skullMaterial;

	ShadowMapGenVS shadowMapCbuffer;
	CB_PS_ShadowMapDraw shadowMapDraw;
	CB_VS_ShadowMapDraw shadowMapVSDraw;
private:
	DemoSwitch currentDemo;

	UINT currentLightNum {};
	DirectX::XMFLOAT3 eyePosition;
	const DirectX::XMMATRIX mirroredSkull = DirectX::XMMatrixTranslation(0.0f, 5.0f, -20.0f);
	ID3D11Buffer* pCopyPCBLightsSkull = nullptr;
	ID3D11Buffer* pCopyVCBMatricesSkull = nullptr;
	ID3D11Buffer* pCopyPCBMirrorSkull = nullptr;
	ID3D11Buffer* pCopySkullMaterial = nullptr;

	ID3D11Buffer* pShadowMapVSDraw = nullptr;
	ID3D11Buffer* pShadomMapGenCB = nullptr;
	ID3D11Buffer* pCopyPCBLightsCylinder = nullptr;
	ID3D11Buffer* pLightDirectionPSCbuffer = nullptr;



};

