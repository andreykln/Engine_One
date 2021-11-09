#pragma once
#include "Shape.h"
class Cylinder : public Shape
{
public:
	Cylinder(Graphics& gfx, float bottom_radius, float top_radius, float height, UINT slice_count, UINT stack_count,
		DemoSwitch in_switch);
	void DrawCylinder(Graphics& gfx, const DirectX::XMMATRIX& in_world,
		const DirectX::XMMATRIX& in_ViewProj, const DirectX::XMFLOAT3 camPositon);
	void UpdateDisplacementCBuffers(Graphics& gfx, const DirectX::XMMATRIX& in_world,
		const DirectX::XMMATRIX& in_ViewProj, const DirectX::XMFLOAT3 in_camera);
	void UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world,
		const DirectX::XMMATRIX& in_ViewProj);
	void UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPositon);
	void IncrementTexArrPos() noexcept;
	void UpdateShadomMapGenBuffers(Graphics& gfx, const DirectX::XMMATRIX& in_lightWorld, DirectX::XMFLOAT3 newCamPosition);
	void UpdateShadowMapDrawBuffers(Graphics& gfx, DirectX::XMFLOAT3 newCamPosition, const DirectX::XMMATRIX& newShadowTransform,
		const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj, ID3D11ShaderResourceView* pShadowMapSRV, 
		DirectX::XMFLOAT3* newLightDirection);
	DirectionalLight* GetOldLightDirection();
private:
	UINT GetTexArrPos() const noexcept;

	DemoSwitch currentDemo;
	GeometryGenerator::MeshData mesh;
	GeometryGenerator cylinderParts;
	CB_PS_DirectionalL_Fog directionalLight;
	CB_VS_Transform transformMatrices;
	CB_PS_PerFrameUpdate pscBuffer;
	CB_CameraPosition_ViewProj displacementMappingCylinderCB;
	CB_VS_TransformWithCameraPosition disMappingVSCB;
	//Shadow map
	ShadowMapGenVS shadowMapCbuffer;
	CB_PS_ShadowMapDraw shadowMapDraw;
	CB_VS_ShadowMapDraw shadowMapVSDraw;

	ID3D11Buffer* pCopyPCBLightsCylinder = nullptr;
	ID3D11Buffer* pCopyLightTextureCylinder = nullptr;
	ID3D11Buffer* pCopyVCBMatricesCylinder = nullptr;
	ID3D11Buffer* pCopyDomainShaderBuffer = nullptr;
	ID3D11Buffer* pShadomMapGenCB = nullptr;
	ID3D11Buffer* pShadowMapVSDraw = nullptr;
	ID3D11Buffer* pLightDirectionPSCbuffer = nullptr;
	UINT texArrPosition{};


};

