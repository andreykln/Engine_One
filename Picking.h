#pragma once
#include "Shape.h"
#include <fstream>

class Picking : public Shape
{
public:
	Picking(Graphics& gfx);
	void UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj);
	void UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPosition);


private:
	CB_VS_Transform transformMatrices;
	CB_PS_DirectionalL_Fog carPSBuffer;
	CB_PS_PerFrameUpdate pscBuffer;

	ID3D11Buffer* pCopyVCBMatricesCar = nullptr;
	ID3D11Buffer* pCopyCarMaterial = nullptr;
	ID3D11Buffer* pCopyPCBLightsPerFrame = nullptr;
};

