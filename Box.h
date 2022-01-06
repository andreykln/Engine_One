#pragma once
#include "Shape.h"

class Box : public Shape
{
public:
	Box(Graphics& gfx, float width, float height, float depth, DemoSwitch demo);
	void UpdateShadomMapGenBuffers(Graphics& gfx, const DirectX::XMMATRIX& in_lightWorld, DirectX::XMFLOAT3 newCamPosition);
	void UpdateShadowMapDrawBuffers(Graphics& gfx, DirectX::XMFLOAT3 newCamPosition, const DirectX::XMMATRIX& newShadowTransform,
		const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProj, ID3D11ShaderResourceView* pShadowMapSRV,
		DirectX::XMFLOAT3& newLightDirection);
	void UpdateNormalMapBuffer(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewM,
		const DirectX::XMMATRIX& in_ViewProjection);
	void DrawTestBox(Graphics& gfx, const DirectX::XMMATRIX& in_world, const DirectX::XMMATRIX& in_ViewProjection, DirectX::XMFLOAT3 camPos,
		Shaders* pShaders);

private:
	DemoSwitch currentDemo;
	ShadowMapGenVS shadowMapCbuffer;
	cbDefaultVS boxVSCB;
	cbDefaultPS boxPSCB;
	cbCreateNormalMap normapMapData;

	ID3D11Buffer* pShadowMapBoxDrawPS = nullptr;
	ID3D11Buffer* pShadowMapVSDraw = nullptr;
	ID3D11Buffer* pNormalMapVSDraw = nullptr;
	ID3D11Buffer* pShadomMapGenCB = nullptr;
	ID3D11ShaderResourceView* pDiffMapHeightMap = nullptr;
	ID3D11ShaderResourceView* pNormalMap = nullptr;
	ID3D11SamplerState* pSamplerState = nullptr;

	//simple box demo
	ID3D11Buffer* pVSCBufferBoxDemo = nullptr;
	ID3D11Buffer* pPSCBufferBoxDemo = nullptr;

	const DirectX::XMMATRIX boxforHillsOffset = DirectX::XMMatrixTranslation(-5.0f, -3.5f, 4.0f);
	GeometryGenerator::MeshData mesh;
	GeometryGenerator box;
};

