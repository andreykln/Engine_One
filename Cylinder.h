#pragma once
#include "Shape.h"

class Cylinder : public Shape
{
public:
	Cylinder(Graphics& gfx, float bottom_radius, float top_radius, float height, UINT slice_count, UINT stack_count);
	ID3D11Buffer** GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	UINT GetIndexCount();

	void UpdateShadowMapGenBuffersInstanced(Graphics& gfx, const DirectX::XMMATRIX& in_lightView);
	void UpdateDrawInstancedBuffers(Graphics& gfx, DirectX::XMFLOAT3 newCamPosition, const DirectX::XMMATRIX& newShadowTransform,
		const DirectX::XMMATRIX& in_ViewProj, ID3D11ShaderResourceView* pShadowMapSRV,
		DirectX::XMFLOAT3& newLightDirection);
	void UpdateNormalMapBuffer(Graphics& gfx, const DirectX::XMMATRIX& in_ViewM,
		const DirectX::XMMATRIX& in_ViewProjection);

	DirectX::XMMATRIX m_CylWorld[10];
private:
	DirectX::XMFLOAT4X4 sCylWorld[10];

	GeometryGenerator::MeshData mesh;
	GeometryGenerator cylinderParts;
	//Shadow map
	ShadowMapGenVS shadowMapCbuffer;
	cbDefaultVS coneVSCB;
	cbDefaultPS conePSCB;
	cbCreateNormalMapInstanced normalMapData;

	ID3D11Buffer* pShadowMapConeDrawPS = nullptr;
	ID3D11Buffer* pShadomMapGenCB = nullptr;
	ID3D11Buffer* pShadowMapVSDraw = nullptr;
	ID3D11Buffer* pNormalMapVSDraw = nullptr;
	ID3D11Buffer* pShadowMapDrawInstancedCB = nullptr;
	ID3D11Buffer* pNormalVB = nullptr;
	ID3D11Buffer* pIAbuffers[2];
	UINT stride[2]{};
	UINT offset[2] = { 0,0 };


	//new architect
	ID3D11Buffer* pVertexBuffer = nullptr;
	ID3D11Buffer* pIndexBuffer = nullptr;
	UINT indexCount = 0;
};

