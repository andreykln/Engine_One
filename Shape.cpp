#include "Shape.h"

float Shape::GetAlpha() const
{
	return alpha;
}

void Shape::Update(float dt) noexcept
{
	alpha = dt;
}

void Shape::AddBind(Bindable* in_bind)
{
	assert("Use AddIndexBuffer for binding IndexBuffer" && typeid(*in_bind) != typeid(IndexBuffer));
	binds.push_back(std::move(in_bind));
}

void Shape::AddIndexBuffer(IndexBuffer* in_bind)
{
 	assert("IndexBuffer already bound" && pIndexBuffer == nullptr);
	pIndexBuffer = in_bind;
	binds.push_back(std::move(in_bind));
}

void Shape::BindAndDrawIndexed(Graphics& gfx) const noexcept
{
	for (auto& b : binds)
	{
		b->Bind(gfx);
	}
	gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Shape::BindAndDraw(Graphics& gfx, UINT VertexCount, UINT StartVertexLocation) const noexcept
{
	for (auto& b : binds)
	{
		b->Bind(gfx);
	}
	gfx.Draw(VertexCount, StartVertexLocation);
}

void Shape::BindAndDrawSeveral(Graphics& gfx, UINT count, UINT startIndexLocation, INT baseVertexLocation) noexcept
{
	for (auto& b : binds)
	{
		b->Bind(gfx);
	}
	gfx.DrawIndexedTwo(count, startIndexLocation, baseVertexLocation);

}

void Shape::DeleteObject() const noexcept
{
	for (auto& b : binds)
	{
		delete b;
	}
}

void Shape::SetCameraMatrix(DirectX::XMMATRIX in_matrix) noexcept
{
	m_Matrix = in_matrix;
}

void Shape::SetVertexShader(Graphics& gfx, ID3D11VertexShader* pVertexShader,std::wstring& path)
{
	gfx.SetVertexShader(pVertexShader, path);
}
