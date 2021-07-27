#pragma once
#include "Shape.h"

class DisplacementWaves : public Shape
{
public:
	DisplacementWaves(Graphics& gfx);
	void UpdateCBs(Graphics& gfx, DirectX::XMMATRIX world, DirectX::XMMATRIX viewProjection,
		DirectX::XMFLOAT3 cameraPosition, float dt);

private:
	GeometryGenerator geoGen;
	GeometryGenerator::MeshData mesh;

	CB_VS_DisplacementWavesPerFrame cbVSPerFrame;
	CB_PS_DirectionalL_Fog cbVSPerObject;
	CB_CameraPosition_ViewProj cbDSCameraViewProj;
	CB_PS_PerFrameUpdate cbPSPerFrame;

	ID3D11Buffer* pCopyVSCBPerFrame = nullptr;
	ID3D11Buffer* pCopyDSCBPerFrame = nullptr;
	ID3D11Buffer* pCopyPSCBPerFrame = nullptr;

	DirectX::XMFLOAT2 waveDispOffset0;
	DirectX::XMFLOAT2 waveDispOffset1;
	DirectX::XMFLOAT2 waveNormalOffset0;
	DirectX::XMFLOAT2 waveNormalOffset1;

	DirectX::XMMATRIX waveDispTexTransform0;
	DirectX::XMMATRIX waveDispTexTransform1;
	DirectX::XMMATRIX waveNormalTransform0;
	DirectX::XMMATRIX waveNormalTransform1;

	ID3D11ShaderResourceView* pSRVPSNormalMap0 = nullptr;
	ID3D11ShaderResourceView* pSRVPSNormalMap1 = nullptr;
	ID3D11ShaderResourceView* pSRVDSHeightMap0 = nullptr;
	ID3D11ShaderResourceView* pSRVDSHeightMap1 = nullptr;




};