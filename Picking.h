#pragma once
#include "Shape.h"
#include <fstream>
#include "DirectXCollision.h"

struct Vertices
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
};

class Picking : public Shape
{
public:
	Picking(Graphics& gfx);
	void UpdateVSMatrices(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj);
	void UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 camPosition);
	void Pick(DirectX::XMMATRIX view, DirectX::XMMATRIX projection, int mX, int mY);
	void SetPickedMaterial(Graphics& gfx);
	int GetPickedTriangleIndex();

private:
	CB_VS_Transform transformMatrices;
	CB_PS_DirectionalL_Fog carPSBuffer;
	CB_PS_PerFrameUpdate pscBuffer;

	ID3D11Buffer* pCopyVCBMatricesCar = nullptr;
	ID3D11Buffer* pCopyCarMaterial = nullptr;
	ID3D11Buffer* pCopyPCBLightsPerFrame = nullptr;

	DirectX::BoundingBox carBox;
	int pickedTriangle = 0;

	//for bounding box
	DirectX::XMFLOAT3 minF3 = { FLT_MAX, FLT_MAX, FLT_MAX };
	DirectX::XMFLOAT3 maxF3 = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
	DirectX::XMVECTOR vMin = DirectX::XMLoadFloat3(&minF3);
	DirectX::XMVECTOR vMax = DirectX::XMLoadFloat3(&maxF3);

	//copies for picking
	std::vector<Vertices> verticesCopy;
	std::vector<UINT> indicesCopy;

};

