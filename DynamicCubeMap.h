#pragma once
#include "Shape.h"
#include "Camera.h"

class DynamicCubeMap : public Shape
{
public:
	DynamicCubeMap(Graphics& gfx);
	void BuildCubeFaceCamera(float x, float y, float z);
	D3D11_VIEWPORT GetViewPort();
	ID3D11DepthStencilView* GetCubeMapDSV();
	ID3D11ShaderResourceView* GetCubeMapSRV();
	ID3D11RenderTargetView* pDynamicCubeMapRTV[6];
	ID3D11ShaderResourceView* pDynamicCubeMapSRV = nullptr;
	DirectX::XMMATRIX cubeFaceProjection[6];

private:
	const int cubeMapSize = 256;
	Camera dynamicCubeCamera[6];


	ID3D11DepthStencilView* pDynamicCubeMapDSV = nullptr;
	D3D11_VIEWPORT dynamicCubeMapViewport;
};