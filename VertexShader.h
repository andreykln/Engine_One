#pragma once
#include "Bindable.h"
class VertexShader : public Bindable
{
public:
	VertexShader(Graphics& gfx, const std::wstring& path);
	void Bind(Graphics& gfx) noexcept override;
	ID3DBlob* GetByteCode() const noexcept; //for InputLayout
private:
	UINT numOfClassInstances{};
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pVertexShaderBlob;
};

