#pragma once
#include "Graphics.h"

enum class ShaderType
{
	Pixel,
	Vertex,
	Compute,
	Domain,
	Hull,
	Geometry
};


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

