#pragma once
#include "Graphics.h"
class Bindable
{
public:
	virtual void Bind(Graphics& gfx) noexcept = 0;
	~Bindable() = default;
protected:
	static ID3D11Device* GetDevice(Graphics& gfx) noexcept;
	static ID3D11DeviceContext* GetContext(Graphics& gfx) noexcept;

};

