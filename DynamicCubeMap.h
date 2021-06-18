#pragma once
#include "Shape.h"

class Camera;

class DynamicCubeMap : public Shape
{
public:
	DynamicCubeMap(Graphics& gfx, Camera* in_camera);

private:
	const int cubeMapSize = 256;

	ID3D11RenderTargetView* pDynamicCubeMapRTV[6];
	ID3D11ShaderResourceView* pDynamicCubeMapSRV = nullptr;
	ID3D11DepthStencilView* pDynamicCubeMapDSV = nullptr;
	D3D11_VIEWPORT dynamicCubeMapViewport;
};