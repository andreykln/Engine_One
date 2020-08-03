#pragma once
#include "Bindable.h"
#include "LightHelper.h"
#include "IndexBuffer.h"
#include "MathHelper.h"
#include <memory>
#include <typeinfo>
#include <cassert>
#include "UtilityStructures.h"
class Shape
{
public:
	Shape()
	{
		fogObj.fogColor = { 0.75f, 0.75f, 0.75f, 1.00f };
		fogObj.fogStartandRange[0] = { 25.0f };
		fogObj.fogStartandRange[1] = { 105.0f };
	}
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
	//can be used for rotation around world axis
	void SetCameraMatrix(DirectX::XMMATRIX in_matrix) noexcept;
	virtual void UpdateVertexConstantBuffer(Graphics& gfx) = 0;
private:
	std::vector<Bindable*> binds;
	const IndexBuffer* pIndexBuffer = nullptr;
protected:
	ID3D11Buffer* pCopyVertexConstantBuffer = nullptr;
	float alpha{};
	CBFog fogObj;
	DirectX::XMMATRIX m_Matrix = DirectX::XMMatrixIdentity();
 	DirectX::XMMATRIX m_Centered = DirectX::XMMatrixTranslation(0.0f, 0.0f, -12.0f);
	
// 	DirectX::XMMATRIX m_Centered = DirectX::XMMatrixIdentity();


};

