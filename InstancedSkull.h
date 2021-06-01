#pragma once
#include "Shape.h"
#include <fstream>
#include "DirectXCollision.h"


struct InstancedData
{
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4 color;
};

class InstancedSkull : public Shape 
{
public:
	InstancedSkull(Graphics& gfx);
	void UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_ViewProj,
		const DirectX::XMMATRIX& in_viewMatrix, const DirectX::XMMATRIX& in_Projection);
	void UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPosition);
	void ComputeFrustumFromProjection(DirectX::XMMATRIX* projection);
	int GetAmountOfVisible() const;
private:
	void BuildInstancedBuffer(std::vector<InstancedData>& data);

	CB_VS_Transform transformMatrices;
	CB_PS_DirectionalL_Fog directionalLight;
	CB_PS_PerFrameUpdate pscBuffer;
	std::vector<InstancedData> instanced;

	ID3D11Buffer* pCopyInstancedBuffer = nullptr;
	ID3D11Buffer* pCopyVCBMatricesSkull = nullptr;
	ID3D11Buffer* pCopyPCBLightsSkull = nullptr;

	//culling
	DirectX::XMFLOAT3 minF3 = { FLT_MAX, FLT_MAX, FLT_MAX };
	DirectX::XMFLOAT3 maxF3 = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
	DirectX::XMVECTOR vMin = DirectX::XMLoadFloat3(&minF3);
	DirectX::XMVECTOR vMax = DirectX::XMLoadFloat3(&maxF3);

	DirectX::BoundingBox skullBox;
	int visibleObjectsCount = 0;
	DirectX::BoundingFrustum cameraFrustum;
	

};
