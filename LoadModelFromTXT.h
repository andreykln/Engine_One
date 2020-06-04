#pragma once
#include "Shape.h"
#include "BindableBase.h"
#include <fstream>

class LoadModelFromTXT : public Shape
{
public:
	LoadModelFromTXT(Graphics& gfx, const std::wstring& path);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	void UpdateVertexConstantBuffer(Graphics& gfx) override;
	void SetCameraMatrix(DirectX::XMMATRIX in_matrix) noexcept;

	//CB structures
	struct CBPerFrame
	{
		CBPerFrame() { ZeroMemory(this, sizeof(this)); }
		DirectionalLight dirLight[3];
		DirectX::XMFLOAT3 cbEyePosition;
		float padding{};
		Material skullMaterial;
	};
	CBPerFrame constBuffPerFrame;
	struct CBPerObject
	{
		CBPerObject() { ZeroMemory(this, sizeof(this)); }
		DirectX::XMMATRIX gWorld;
		DirectX::XMMATRIX gWorldInvTranspose;
		DirectX::XMMATRIX gWorldViewProj;
	};
	CBPerObject constBuffPerObject;

private:
	ID3D11Buffer* pCopyPCBPerFrame = nullptr;
	ID3D11Buffer* pCopyVCBPerObject = nullptr;


};

