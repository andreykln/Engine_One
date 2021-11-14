#pragma once
#include "Shape.h"

class GeoSphere : public Shape
{
public:
	GeoSphere(Graphics& gfx, float radius, UINT numSubdivisions, bool in_centerSphere, DemoSwitch in_switch);
	void DrawSpheres(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj, float dt,
		DirectX::XMFLOAT3 camPositon);
	void UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj, float dt);
	void UpdateShadomMapGenBuffers(Graphics& gfx, const DirectX::XMMATRIX& in_lightWorld, DirectX::XMFLOAT3 newCamPosition);
	void UpdateShadowMapDrawBuffers(Graphics& gfx, DirectX::XMFLOAT3 newCamPosition, const DirectX::XMMATRIX& newShadowTransform,
		const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj, ID3D11ShaderResourceView* pShadowMapSRV,
		DirectX::XMFLOAT3* newLightDirection);
	void UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPositon);
private:
	DirectX::XMFLOAT2 sphereTextureOffset;
	DirectX::XMMATRIX sphereOffset;
	DemoSwitch currentDemo;
	ShadowMapGenVS shadowMapCbuffer;
	CB_PS_ShadowMapDraw shadowMapDraw;
	CB_VS_ShadowMapDraw shadowMapVSDraw;


	GeometryGenerator::MeshData mesh;
	GeometryGenerator sphere;
	bool centerSphere;

	CB_PS_DirectionalL_Fog directionalLight;
	CB_VS_Transform transformMatrices;
	CB_PS_PerFrameUpdate pscBuffer;

	ID3D11Buffer* pCopyPCBLightsGeoSphere = nullptr;
	ID3D11Buffer* pCopyVCBMatricesGeoSphere = nullptr;

	//Shadow
	ID3D11Buffer* pShadowMapVSDraw = nullptr;
	ID3D11Buffer* pShadomMapGenCB = nullptr;
	ID3D11Buffer* pCopyPCBLightsCylinder = nullptr;
	ID3D11Buffer* pLightDirectionPSCbuffer = nullptr;
};

