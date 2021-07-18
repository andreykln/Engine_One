#pragma once
#include "Shape.h"


class TreeBillboard : public Shape 
{
public:
	TreeBillboard(Graphics& gfx);
	void UpdateConstantBuffers(Graphics& gfx, const DirectX::XMMATRIX& in_ViewProj, DirectX::XMFLOAT3 cameraPosition);
private:
	void UpdateGSConstBuffer(Graphics& gfx, const DirectX::XMMATRIX& in_ViewProj, DirectX::XMFLOAT3 cameraPosition);
	void UpdatePSConstBuffers(Graphics& gfx, DirectX::XMFLOAT3 cameraPosition);
	CBBillboardGeometry CBGeometryShader;////
	CB_PS_DirectionalL_Fog directionalLight;
	CB_PS_PerFrameUpdate pscBuffer;
	ID3D11Buffer* pCopyGSConstBuffer = nullptr;
	ID3D11Buffer* pCopyPSConstBuffer = nullptr;
	ID3DBlob* pVertexShaderBlob = nullptr;


};