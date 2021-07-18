#pragma once

#include "Shape.h"

struct MirrorRoomCB
{
	Material objectMaterial;
	DirectX::XMFLOAT3 cbEyePosition;
	//int numLights = { 3 };
	UINT currentTexture;
	//unsigned int pad[3];
};


struct TextureSwitcher
{
	Material mat;
	UINT currentTexture = 1u;
	UINT pad0;
	UINT pad1;
	UINT pad2;

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
	void UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj);
	void UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPositon);
	void SwitchTexture(Graphics& gfx, UINT texArrPos);
private:
	CB_VS_Transform transformMatrices;
	CB_PS_MirrorRoom pscBuffer;
	TextureSwitcher tSwitcher;

	MirrorRoomCB testCB;
	LightSwitcher switcher;
	Material mirrorMaterial;
	Material floorMaterial;
	Material wallMaterial;



	ID3D11Buffer* pCopyMirrorCBuffer = nullptr;

	ID3D11Buffer* pCopyMirrorRoomCB = nullptr;
	ID3D11Buffer* pCopyLightSwitcher = nullptr;
	ID3D11Buffer* pCopyVCBMatricesMirror = nullptr;
	ID3D11Buffer* pCopyTexSwitcher = nullptr;

};

