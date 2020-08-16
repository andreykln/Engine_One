#pragma once

#include "Shape.h"
#include "BindableBase.h"


struct MirrorRoomCB
{
	Material objectMaterial;
	DirectX::XMFLOAT3 cbEyePosition;
	//int numLights = { 3 };
	UINT currentTexture;
	//unsigned int pad[3];
};


struct LightSwitcher
{
	UINT numLights = { 3 };
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
	LightSwitcher switcher;
	Material mirrorMaterial;
	Material floorMaterial;
	Material wallMaterial;
	CBPerFrameMirrorRoom constLights;
	CBPerObjectTexture constMatrices;
	ID3D11Buffer* pCopyPCBLightsMirror = nullptr;
	ID3D11Buffer* pCopyMirrorRoomCB = nullptr;
	ID3D11Buffer* pCopyLightSwitcher = nullptr;
	ID3D11Buffer* pCopyVCBMatricesMirror = nullptr;

};

