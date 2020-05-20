#pragma once
#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"
#include "Waves.h"
#include "MathHelper.h"
class HillsAndWaves : public Shape
{
public:
	HillsAndWaves(Graphics& gfx);
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	void UpdateVertexConstantBuffer(Graphics& gfx) override;
	void UpdateScene(float totalTime, float dt, Graphics& gfx);
	UINT GetHillsIndicesNum() noexcept;
	UINT GetWavesIndicesNum() noexcept;
	UINT GetHillsVerticesNum() noexcept;
private:
	struct Vertex_HAW
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
	};
	UINT wavesIndicesNum{};
	std::vector<Vertex_HAW> verticesHills;
	std::vector<Vertex_HAW> verticesWaves;
	float GetHeight(float x, float z) const;
	GeometryGenerator::MeshData meshHills;
	GeometryGenerator::MeshData meshWaves;
	GeometryGenerator generator;
	VertexBufferDynamic* pDynamicVB = nullptr;
	ID3D11Buffer* pCopyDynamicVB = nullptr;
	ID3D11Buffer* pCopyVertexConstantBuffer = nullptr;
	Waves wave;

};

