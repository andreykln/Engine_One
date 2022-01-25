#pragma once
#include "Shape.h"

class Skull
{
public:
	Skull(Graphics& gfx, const std::wstring& path);
	ID3D11Buffer** GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	UINT GetIndexCount();

	MaterialEx skullMaterial;
	//anything for those two, they are not used for the skull
	const std::wstring diffuseMap = L"bricks3";
	const std::wstring normalMap = L"bricks3_nmap";
	const DirectX::XMMATRIX skullWorld =DirectX::XMMatrixTranslation(0.0f, 6.0f, 0.0f) * DirectX::XMMatrixScaling(0.3f, 0.3f, 0.3f);
private:
	UINT indexCount = 0;
	ID3D11Buffer* pVertexBuffer = nullptr;
	ID3D11Buffer* pIndexBuffer = nullptr;
};

