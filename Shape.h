#pragma once
#include "Bindable.h"
#include "IndexBuffer.h"
#include <memory>
#include <typeinfo>
#include <cassert>

//class IndexBuffer;
class Shape
{
public:
	Shape() = default;
	Shape(const Shape&) = delete;
	virtual ~Shape() = default;
	virtual DirectX::XMMATRIX GetTransform() const noexcept = 0;
	float GetAlpha() const;
	virtual void Update(float dt) noexcept = 0;
	void AddBind(Bindable* in_bind);
	void AddIndexBuffer(IndexBuffer* in_bind);
	void BindAndDraw(Graphics& gfx) const noexcept;
	void BindAndDrawSeveral(Graphics& gfx, UINT count, UINT startIndexLocation, INT baseVertexLocation) noexcept;
	void DeleteObject() const noexcept;
	void SetMatrix(DirectX::XMMATRIX in_matrix) noexcept;
	virtual void UpdateVertexConstantBuffer(Graphics& gfx) = 0;
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

