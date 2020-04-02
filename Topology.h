#pragma once
#include "Bindable.h"
class Topology : public Bindable
{
public:
	Topology(Graphics& gfx, D3D_PRIMITIVE_TOPOLOGY in_type) noexcept;
	void Bind(Graphics& gfx) noexcept override;
private:
	D3D_PRIMITIVE_TOPOLOGY type;
};

