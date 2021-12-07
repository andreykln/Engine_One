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
	void UpdateShadowMapGenBuffersInstanced(Graphics& gfx, const DirectX::XMMATRIX& in_lightView);
	void UpdateShadowMapDrawBuffers(Graphics& gfx, DirectX::XMFLOAT3 newCamPosition, const DirectX::XMMATRIX& newShadowTransform,
		const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj, ID3D11ShaderResourceView* pShadowMapSRV, 
		DirectX::XMFLOAT3& newLightDirection);
	void UpdatePosition(Graphics& gfx, const DirectX::XMMATRIX& in_world);
	DirectionalLight* GetOldLightDirection();
private:
	UINT GetTexArrPos() const noexcept;
	DirectX::XMFLOAT4X4 sCylWorld[10];
	DirectX::XMMATRIX m_CylWorld[10];

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
	cbDefaultVS coneVSCB;
	cbDefaultPS conePSCB;

	ID3D11Buffer* pShadowMapConeDrawPS = nullptr;

	ID3D11Buffer* pCopyPCBLightsCylinder = nullptr;
	ID3D11Buffer* pCopyLightTextureCylinder = nullptr;
	ID3D11Buffer* pCopyVCBMatricesCylinder = nullptr;
	ID3D11Buffer* pCopyDomainShaderBuffer = nullptr;
	ID3D11Buffer* pShadomMapGenCB = nullptr;
	ID3D11Buffer* pShadowMapVSDraw = nullptr;
	ID3D11Buffer* pLightDirectionPSCbuffer = nullptr;
	UINT texArrPosition{};


	ID3D11Buffer* pNormalVB = nullptr;
	ID3D11Buffer* pIAbuffers[2];
	UINT stride[2]{};

};

