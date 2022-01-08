#pragma once
#include "Shape.h"

class Sky : public Shape
{
public:
	Sky(Graphics& gfx);
	ID3D11Buffer** GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	UINT GetIndexCount();

	//grasscube1024 desertcube1024 sunsetcube1024 snowcube1024
	std::wstring skyBoxName = L"grasscube1024";
private:
	GeometryGenerator geoGen;
	GeometryGenerator::MeshData mesh;
	ID3D11Buffer* pVertexBuffer = nullptr;
	ID3D11Buffer* pIndexBuffer = nullptr;
	UINT indexCount = 0;


};