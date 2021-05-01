#pragma once

#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"
//#include "directxpackedvector.h" //for XMCOLOR

class Box : public Shape
{
public:
	Box(Graphics& gfx, float width, float height, float depth, DemoSwitch demo);
	DirectX::XMMATRIX GetBoxForHillsOffset();
	void UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj);
	void UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPositon);

private:
	CB_VS_Transform transformMatrices;
	CB_PS_DirectionalL_Fog directionalLight;
	CB_PS_PerFrameUpdate pscBuffer;

	DemoSwitch currentDemo;
	GeometryGenerator::MeshData mesh;
	GeometryGenerator box;
	ID3D11Buffer* pCopyPCBLightsBox = nullptr;
	ID3D11Buffer* pCopyVCBMatricesBox = nullptr;

	const DirectX::XMMATRIX boxforHillsOffset = DirectX::XMMatrixTranslation(-5.0f, -3.5f, 4.0f);

};

