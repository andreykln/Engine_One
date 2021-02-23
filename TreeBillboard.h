#pragma once
#include "Shape.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"


class TreeBillboard : public Shape 
{
public:
	TreeBillboard(Graphics& gfx, std::vector<TreePointSprite> in_trees_pos);

	DirectX::XMMATRIX GetTransform() const noexcept override;
	void Update(float dt) noexcept override;
	void UpdateVertexConstantBuffer(Graphics& gfx) override;
private:
	std::vector<TreePointSprite> treePositions;
	ID3DBlob* pVertexShaderBlob = nullptr;


};