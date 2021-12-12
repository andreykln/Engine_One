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
private:
	DemoSwitch currentDemo;
	ShadowMapGenVS shadowMapCbuffer;
	cbDefaultVS boxVSCB;
	cbDefaultPS boxPSCB;

	ID3D11Buffer* pShadowMapBoxDrawPS = nullptr;
	ID3D11Buffer* pShadowMapVSDraw = nullptr;
	ID3D11Buffer* pShadomMapGenCB = nullptr;
	ID3D11ShaderResourceView* pDiffMapHeightMap = nullptr;
	ID3D11ShaderResourceView* pNormalMap = nullptr;
	ID3D11SamplerState* pSamplerState = nullptr;



	const DirectX::XMMATRIX boxforHillsOffset = DirectX::XMMatrixTranslation(-5.0f, -3.5f, 4.0f);
	GeometryGenerator::MeshData mesh;
	GeometryGenerator box;
};

