#include "PixelShader.h"

PixelShader::PixelShader(Graphics& gfx, const std::wstring& path)
{
	Microsoft::WRL::ComPtr<ID3DBlob> pPixelShaderBlob;
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), pPixelShaderBlob.ReleaseAndGetAddressOf())); 
	DX::ThrowIfFailed(GetDevice(gfx)->CreatePixelShader(
		pPixelShaderBlob->GetBufferPointer(),
		pPixelShaderBlob->GetBufferSize(),
		nullptr,
		pPixelShader.ReleaseAndGetAddressOf()));
}

void PixelShader::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, numOfClassInstances);
}
