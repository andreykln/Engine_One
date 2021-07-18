#pragma once
#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"
class Cylinder : public Shape
{
public:
	Cylinder(Graphics& gfx, float bottom_radius, float top_radius, float height, UINT slice_count, UINT stack_count,
		DemoSwitch in_switch);
	void UpdateDisplacementCBuffers(Graphics& gfx, const DirectX::XMMATRIX& in_world,
		const DirectX::XMMATRIX& in_ViewProj, const DirectX::XMFLOAT3 in_camera);
	void UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world,
		const DirectX::XMMATRIX& in_ViewProj);
	void UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPositon);
	void IncrementTexArrPos() noexcept;
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

	ID3D11Buffer* pCopyPCBLightsCylinder = nullptr;
	ID3D11Buffer* pCopyLightTextureCylinder = nullptr;
	ID3D11Buffer* pCopyVCBMatricesCylinder = nullptr;
	ID3D11Buffer* pCopyDomainShaderBuffer = nullptr;

	UINT texArrPosition{};


};

