#pragma once
#include "Shape.h"
#include <fstream>

struct InstancedData
{
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4 color;
};

class InstancedSkull : public Shape 
{
public:
	InstancedSkull(Graphics& gfx);
	void UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_ViewProj);
	void UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPosition);


private:
	void BuildInstancedBuffer(std::vector<InstancedData>& data);

	CB_VS_Transform transformMatrices;
	CB_PS_DirectionalL_Fog directionalLight;
	CB_PS_PerFrameUpdate pscBuffer;
	std::vector<InstancedData> instanced;

	ID3D11Buffer* pCopyInstancedBuffer = nullptr;
	ID3D11Buffer* pCopyVCBMatricesSkull = nullptr;
	ID3D11Buffer* pCopyPCBLightsSkull = nullptr;
};
