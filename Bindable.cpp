#include "Bindable.h"

#ifdef MY_DEBUG
ID3D11Debug* Bindable::GetDebug(Graphics& gfx) noexcept
{
	return gfx.debugDevice;
}
#endif // MY_DEBUG


ID3D11Device* Bindable::GetDevice(Graphics& gfx) noexcept
{
	return gfx.pgfx_pDevice.Get();
}

ID3D11DeviceContext* Bindable::GetContext(Graphics& gfx) noexcept
{
	return gfx.pgfx_pDeviceContext.Get();
}
