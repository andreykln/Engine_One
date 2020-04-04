#include "Bindable.h"

ID3D11Debug* Bindable::GetDebug(Graphics& gfx) noexcept
{
	return gfx.debugDevice;
}

ID3D11Device* Bindable::GetDevice(Graphics& gfx) noexcept
{
	return gfx.pgfx_pDevice.Get();
}

ID3D11DeviceContext* Bindable::GetContext(Graphics& gfx) noexcept
{
	return gfx.pgfx_pDeviceContext.Get();
}
