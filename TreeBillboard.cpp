#include "TreeBillboard.h"

TreeBillboard::TreeBillboard(Graphics& gfx, std::vector<TreePointSprite> in_trees_pos)
	: treePositions(in_trees_pos)
{
	VertexBuffer* pVertexBuffer = new VertexBuffer(gfx, treePositions, L"TreeSpritePositions");
	AddBind(pVertexBuffer);

	VertexShader* pVertexShader = new VertexShader(gfx, L"Shaders\\Vertex\\TreeBillboardVS.cso");
	pVertexShaderBlob = pVertexShader->GetByteCode();
	AddBind(pVertexShader);

	InputLayout* pInputLayout = new InputLayout(gfx, pVertexShaderBlob, treeBbrdLayout, L"TreeBilboardInLayout");
	AddBind(pInputLayout);



}

DirectX::XMMATRIX TreeBillboard::GetTransform() const noexcept
{
	return m_Matrix * m_Centered;;
}

void TreeBillboard::Update(float dt) noexcept
{

}

void TreeBillboard::UpdateVertexConstantBuffer(Graphics& gfx)
{

}
