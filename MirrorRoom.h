#pragma once

#include "Shape.h"
#include "BindableBase.h"


class MirrorRoom : public Shape
{
public:
	MirrorRoom(Graphics& gfx);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	void UpdateVertexConstantBuffer(Graphics& gfx) override;
private:
	CBPerFrame constLights;
	CBPerObjectTexture constMatrices;
	ID3D11Buffer* pCopyPCBLightsMirror = nullptr;
	ID3D11Buffer* pCopyVCBMatricesMirror = nullptr;

};

