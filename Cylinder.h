#pragma once
#include "Shape.h"
class Cylinder : public Shape
{
public:
	Cylinder(Graphics& gfx, float bottom_radius, float top_radius, float height, UINT slice_count, UINT stack_count);
	void UpdateShadowMapGenBuffersInstanced(Graphics& gfx, const DirectX::XMMATRIX& in_lightView);
	void UpdateDrawInstancedBuffers(Graphics& gfx, DirectX::XMFLOAT3 newCamPosition, const DirectX::XMMATRIX& newShadowTransform,
		const DirectX::XMMATRIX& in_ViewProj, ID3D11ShaderResourceView* pShadowMapSRV,
		DirectX::XMFLOAT3& newLightDirection);
private:
	DirectX::XMFLOAT4X4 sCylWorld[10];
	DirectX::XMMATRIX m_CylWorld[10];

	GeometryGenerator::MeshData mesh;
	GeometryGenerator cylinderParts;
	//Shadow map
	ShadowMapGenVS shadowMapCbuffer;
	cbDefaultVS coneVSCB;
	cbDefaultPS conePSCB;
	ID3D11Buffer* pShadowMapConeDrawPS = nullptr;
	ID3D11Buffer* pShadomMapGenCB = nullptr;
	ID3D11Buffer* pShadowMapVSDraw = nullptr;
	ID3D11Buffer* pShadowMapDrawInstancedCB = nullptr;
	ID3D11Buffer* pNormalVB = nullptr;
	ID3D11Buffer* pIAbuffers[2];
	UINT stride[2]{};
	UINT offset[2] = { 0,0 };


};

