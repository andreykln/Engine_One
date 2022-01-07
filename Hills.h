#pragma once
#include "Shape.h"
#include "LightHelper.h"
#include "MathHelper.h"
class Hills : public Shape
{
public:
	Hills(Graphics& gfx, float in_width, float in_depth, UINT in_m, UINT in_n);
	ID3D11Buffer** GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	UINT GetIndexCount();



	void UpdateShadomMapGenBuffers(Graphics& gfx, const DirectX::XMMATRIX& in_lightWorld, DirectX::XMFLOAT3 newCamPosition);
	void UpdateShadowMapDrawBuffers(Graphics& gfx, DirectX::XMFLOAT3 newCamPosition, const DirectX::XMMATRIX& newShadowTransform,
		const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj, ID3D11ShaderResourceView* pShadowMapSRV,
		DirectX::XMFLOAT3& newLightDirection);
	void UpdateNormalMapBuffer(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewM,
		const DirectX::XMMATRIX& in_ViewProjection);

private:
	float width{};
	float depth{};
	UINT m{};
	UINT n{};
	const DirectX::XMMATRIX plateScaling = DirectX::XMMatrixScaling(10.0f, 10.0f, 10.0f);

	ID3D11Buffer* pShadomMapGenCB = nullptr;
	ID3D11Buffer* pShadowMapVSDraw = nullptr;
	ID3D11Buffer* pNormalMapDraw = nullptr;
	ID3D11Buffer* pPlaneDrawPS = nullptr;

	CB_VS_ShadowMapDraw shadowMapVSDraw;
	CB_PS_ShadowMapDraw shadowMapDraw;
	cbDefaultVS planeVSCB;
	cbDefaultPS planePSCB;
	cbCreateNormalMap normalMapData;

	GeometryGenerator::MeshData grid;
	GeometryGenerator landscapeGenerated;
	ShadowMapGenVS shadowMapCbuffer;


	//new architect
	ID3D11Buffer* pVertexBuffer = nullptr;
	ID3D11Buffer* pIndexBuffer = nullptr;
	UINT indexCount = 0;
};

