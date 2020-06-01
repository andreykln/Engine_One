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
public:
//	float alpha{};


};

