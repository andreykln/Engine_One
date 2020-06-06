#pragma once
#include "Shape.h"
#include "BindableBase.h"
#include <fstream>

class Skull : public Shape
{
public:
	Skull(Graphics& gfx, const std::wstring& path);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	void UpdateVertexConstantBuffer(Graphics& gfx) override;
	void SetCameraMatrix(DirectX::XMMATRIX in_matrix) noexcept;

	CBPerFrame constBuffPerFrame;
	CBPerObject constBuffPerObject;

private:
	ID3D11Buffer* pCopyPCBLightsSkull = nullptr;
	ID3D11Buffer* pCopyVCBMatricesSkull = nullptr;


};

