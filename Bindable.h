#pragma once
#include "Graphics.h"
class Bindable
{
public:
	virtual void Bind(Graphics& gfx) noexcept = 0;
	virtual ~Bindable() = default;
protected:
	
#ifdef MY_DEBUG
	static ID3D11Debug* GetDebug(Graphics& gfx) noexcept;
#endif
	static ID3D11Device* GetDevice(Graphics& gfx) noexcept;
	static ID3D11DeviceContext* GetContext(Graphics& gfx) noexcept;

};

