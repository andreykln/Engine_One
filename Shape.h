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
	virtual void Update(float dt) noexcept = 0;
	void AddBind(std::unique_ptr<Bindable> in_bind);
	void AddIndexBuffer(std::unique_ptr<IndexBuffer> in_bind);
	void BindAndDraw(Graphics& gfx) const noexcept;
	void SetMatrix(DirectX::XMMATRIX in_matrix) noexcept;
private:
	std::vector<std::unique_ptr<Bindable>> binds;
	const IndexBuffer* pIndexBuffer = nullptr;
public:
	float alpha{};
	DirectX::XMMATRIX m_Matrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_Centered = DirectX::XMMatrixTranslation(0.0f, 0.0f, 4.0f);


};

