#pragma once
#include "Shape.h"

class Skull : public Shape
{
public:
	Skull(Graphics& gfx, const std::wstring& path);
	void UpdateShadomMapGenBuffers(Graphics& gfx, const DirectX::XMMATRIX& in_lightWorld, DirectX::XMFLOAT3 newCamPosition);
	void UpdateShadowMapDrawBuffers(Graphics& gfx, DirectX::XMFLOAT3 newCamPosition, const DirectX::XMMATRIX& newShadowTransform,
		const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj, ID3D11ShaderResourceView* pShadowMapSRV,
		DirectX::XMFLOAT3& newLightDirection);

	Material shadowMaterial;
	Material skullMatData;
	CB_PS_DirectionalEX_Fog dirLightEX;

	ShadowMapGenVS shadowMapCbuffer;
	CB_PS_ShadowMapDraw shadowMapDraw;
	CB_VS_ShadowMapDraw shadowMapVSDraw;
private:
	ID3D11Buffer* pShadowMapVSDraw = nullptr;
	ID3D11Buffer* pShadomMapGenCB = nullptr;
	ID3D11Buffer* pCopyPCBLightsCylinder = nullptr;
	ID3D11Buffer* pLightDirectionPSCbuffer = nullptr;
};

