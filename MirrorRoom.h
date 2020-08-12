#pragma once

#include "Shape.h"
#include "BindableBase.h"


struct MirrorRoomCB
{
	UINT currentTexture;
	UINT pad[3];
};

class MirrorRoom : public Shape
{
public:
	MirrorRoom(Graphics& gfx);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	void UpdateVertexConstantBuffer(Graphics& gfx) override;
	void UpdateMirrorRoomConstBuffers(Graphics& gfx, UINT texture);
private:
	MirrorRoomCB testCB;
	Material mirrorMaterial;
	Material floorMaterial;
	CBPerFrameMirrorRoom constLights;
	CBPerObjectTexture constMatrices;
	ID3D11Buffer* pCopyPCBLightsMirror = nullptr;
	ID3D11Buffer* pCopyMirrorRoomCB = nullptr;

	ID3D11Buffer* pCopyVCBMatricesMirror = nullptr;

};

