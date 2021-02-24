#pragma once
#include "Bindable.h"

class GeometryShader : public Bindable
{
public:
	GeometryShader(Graphics& gfx, const std::wstring& path);
	void Bind(Graphics& gfx) noexcept override;
private:
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> pGeometryShader;
	UINT numClassInstances = 0;
};