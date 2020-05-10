#pragma once
#include "Shape.h"
#include "Waves.h"
#include "BindableBase.h"
#include "MathHelper.h"
#include "GeometryGenerator.h"


class WaveSurface : public Shape
{
public:
	struct Vertex
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT4 Color;
	};
	WaveSurface(Graphics& gfx);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	void UpdateScene(float totalTime, float dt, Graphics& gfx);
public:
//	float alpha{};
private:
	std::vector<Vertex> vertices;
	Waves wave;
	VertexBufferDynamic* pDynamicVB;
	ID3D11Buffer* pCopyDynamicVB;



	GeometryGenerator::MeshData mesh;
	GeometryGenerator sphere;

};

