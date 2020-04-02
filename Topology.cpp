#include "Topology.h"

Topology::Topology(Graphics& gfx, D3D_PRIMITIVE_TOPOLOGY in_type) noexcept
	: type(in_type)
{
}

void Topology::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetPrimitiveTopology(type);
}
