#pragma once
#include "Shape.h"

class GeoSphere : public Shape
{
public:
	GeoSphere(Graphics& gfx, float radius, UINT numSubdivisions, bool in_centerSphere, DemoSwitch in_switch);
	ID3D11Buffer** GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	UINT GetIndexCount();


	void UpdateShadowMapGenBuffersInstanced(Graphics& gfx, const DirectX::XMMATRIX& in_lightView);
	void UpdateShadowMapDrawInstancedBuffers(Graphics& gfx, DirectX::XMFLOAT3 newCamPosition, const DirectX::XMMATRIX& newShadowTransform,
		const DirectX::XMMATRIX& in_ViewProj, ID3D11ShaderResourceView* pShadowMapSRV,
		DirectX::XMFLOAT3& newLightDirection);
	void UpdateNormalMapBuffer(Graphics& gfx, const DirectX::XMMATRIX& in_ViewM,
		const DirectX::XMMATRIX& in_ViewProjection);



	DirectX::XMMATRIX m_SphereWorld[10];
private:
	DirectX::XMFLOAT2 sphereTextureOffset;
	DirectX::XMMATRIX sphereOffset;
	DemoSwitch currentDemo;
	ShadowMapGenVS shadowMapCbuffer;
	CB_PS_ShadowMapDraw shadowMapDraw;
	CB_VS_ShadowMapDraw shadowMapVSDraw;
	cbDefaultVS geoSphereVSCB;
	cbDefaultPS geoSpherePSCB;
	cbCreateNormalMapInstanced normalMapData;

	GeometryGenerator::MeshData mesh;
	GeometryGenerator sphere;
	bool centerSphere;

	CB_PS_DirectionalL_Fog directionalLight;
	CB_VS_Transform transformMatrices;
	CB_PS_PerFrameUpdate pscBuffer;

	ID3D11Buffer* pCopyPCBLightsGeoSphere = nullptr;
	ID3D11Buffer* pCopyVCBMatricesGeoSphere = nullptr;
	ID3D11Buffer* pNormalMapVSDraw = nullptr;
	//Shadow
	ID3D11Buffer* pShadowMapVSDraw = nullptr;
	ID3D11Buffer* pShadomMapGenCB = nullptr;
	ID3D11Buffer* pLightGeoSphere = nullptr;
	ID3D11Buffer* pLightDirectionPSCbuffer = nullptr;

	DirectX::XMFLOAT4X4 sGeoSphereWorld[10];
	ID3D11Buffer* pIAbuffers[2];
	UINT stride[2]{};
	UINT offset[2] = { 0,0 };



	//new architect
	ID3D11Buffer* pVertexBuffer = nullptr;
	ID3D11Buffer* pIndexBuffer = nullptr;
	UINT indexCount = 0;

};

