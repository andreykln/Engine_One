#pragma once

#include "Shape.h"

class Box : public Shape
{
public:
	Box(Graphics& gfx, float width, float height, float depth, DemoSwitch demo);
	void DrawBox(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj,
		DirectX::XMFLOAT3 camPositon);
	DirectX::XMMATRIX GetBoxForHillsOffset();
	void UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj);
	void UpdateDisplacementCBuffers(Graphics& gfx, const DirectX::XMMATRIX& in_world,
		const DirectX::XMMATRIX& in_ViewProj, const DirectX::XMFLOAT3 in_camera);
	void UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPositon);

	void UpdateShadomMapGenBuffers(Graphics& gfx, const DirectX::XMMATRIX& in_lightWorld, DirectX::XMFLOAT3 newCamPosition);
	void UpdateShadowMapDrawBuffers(Graphics& gfx, DirectX::XMFLOAT3 newCamPosition, const DirectX::XMMATRIX& newShadowTransform,
		const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj, ID3D11ShaderResourceView* pShadowMapSRV,
		DirectX::XMFLOAT3* newLightDirection);


private:
	CB_VS_Transform transformMatrices;
	CB_PS_DirectionalL_Fog directionalLight;
	CB_PS_PerFrameUpdate pscBuffer;
	CB_VS_TransformWithCameraPosition displacementMappingVS_CB;
	CB_CameraPosition_ViewProj displacementMappingDS_CB;

	ShadowMapGenVS shadowMapCbuffer;
	CB_PS_ShadowMapDraw shadowMapDraw;
	CB_VS_ShadowMapDraw shadowMapVSDraw;

	DemoSwitch currentDemo;
	GeometryGenerator::MeshData mesh;
	GeometryGenerator box;
	ID3D11Buffer* pCopyPCBLightsBox = nullptr;
	ID3D11Buffer* pCopyVCBMatricesBox = nullptr;
	ID3D11Buffer* pCopyDSdisMappingCB = nullptr;
	ID3D11Buffer* pCopyVSdisMappingCb = nullptr;

	//Shadow
	ID3D11Buffer* pShadowMapVSDraw = nullptr;
	ID3D11Buffer* pShadomMapGenCB = nullptr;
	ID3D11Buffer* pCopyPCBLightsCylinder = nullptr;
	ID3D11Buffer* pLightDirectionPSCbuffer = nullptr;
	ID3D11Buffer* pCopyLightPerFrame = nullptr;

	const DirectX::XMMATRIX boxforHillsOffset = DirectX::XMMatrixTranslation(-5.0f, -3.5f, 4.0f);

};

