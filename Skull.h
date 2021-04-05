#pragma once
#include "Shape.h"
#include "BindableBase.h"
#include <fstream>

class Skull : public Shape
{
public:
	Skull(Graphics& gfx, const std::wstring& path, DemoSwitch in_currentDemo);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	void UpdateVertexConstantBuffer(Graphics& gfx) override;
	//updating only light for mirror demo
	void UpdateLightDirection(Graphics& gfx);
	void SetCameraMatrix(DirectX::XMMATRIX in_matrix) noexcept;

	void UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj);
	void UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPosition);
	void UpdatePSLightDirection(Graphics& gfx, DirectX::XMFLOAT3 lightDirection, UINT index);
	DirectX::XMMATRIX GetMirroredSkullTranslation() const;
	void SetNewLightDirection(DirectX::XMFLOAT3& lightDirection, UINT index) noexcept;
	void SetNewLightDirection_(DirectX::XMFLOAT3 lightDirection[3], UINT index) noexcept;
	void UpdateEyePosition(DirectX::XMFLOAT3 eyePos) noexcept;
	void UpdateMaterial(Graphics& gfx, bool shadow) noexcept;
	DirectionalLight GetLightDirection(UINT index) const noexcept;
	Material shadowMaterial;
	static CBPerFrame constBuffPerFrame;
	CBPerObject constBuffPerObject;


	CB_VS_Transform transformMatrices;
	CB_PS_DirectionalL_Fog directionalLight;
	CB_PS_PerFrameUpdate pscBuffer;
	CB_PS_Skull_Mirror mirrorBuffer;
private:
	DemoSwitch currentDemo;

	UINT currentLightNum {};
	DirectX::XMFLOAT3 eyePosition;
	const DirectX::XMMATRIX mirroredSkull = DirectX::XMMatrixTranslation(0.0f, 5.0f, -20.0f);
	ID3D11Buffer* pCopyPCBLightsSkull = nullptr;
	ID3D11Buffer* pCopyVCBMatricesSkull = nullptr;
	ID3D11Buffer* pCopyPCBMirrorSkull = nullptr;


};

