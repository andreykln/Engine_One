#pragma once
#include "Bindable.h"
#include "GeometryGenerator.h"
#include "BindableBase.h"
#include "MathHelper.h"
#include <memory>
#include <typeinfo>
#include <cassert>
#include "UtilityStructures.h"
class Shape
{
public:
	Shape()	{}
	Shape(const Shape&) = delete;
	virtual ~Shape() = default;
	void AddBind(Bindable* in_bind);
	void AddIndexBuffer(IndexBuffer* in_bind);
	void BindAndDrawIndexed(Graphics& gfx) const noexcept;
	void BindAndDraw(Graphics& gfx, UINT VertexCount, UINT StartVertexLocation) const noexcept;
	void BindAndDrawSeveral(Graphics& gfx, UINT count, UINT startIndexLocation, INT baseVertexLocation) noexcept;
	void DeleteObject() const noexcept;
	//can be used for rotation around world axis

private:
	std::vector<Bindable*> binds;
	const IndexBuffer* pIndexBuffer = nullptr;
protected:
	ID3D11Buffer* pCopyVertexConstantBuffer = nullptr;
	float alpha{};
	DirectX::XMMATRIX m_Matrix = DirectX::XMMatrixIdentity();
 	DirectX::XMMATRIX m_Centered = DirectX::XMMatrixTranslation(0.0f, 0.0f, -12.0f);
	
// 	DirectX::XMMATRIX m_Centered = DirectX::XMMatrixIdentity();


};

