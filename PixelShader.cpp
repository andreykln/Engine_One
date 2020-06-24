#include "PixelShader.h"

PixelShader::PixelShader(Graphics& gfx, const std::wstring& path)
{
#ifdef MY_DEBUG
	gfx.CheckFileExistence(gfx, path);
#endif // MY_DEBUG
	Microsoft::WRL::ComPtr<ID3DBlob> pPixelShaderBlob;
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), pPixelShaderBlob.ReleaseAndGetAddressOf())); 
	DX::ThrowIfFailed(GetDevice(gfx)->CreatePixelShader(
		pPixelShaderBlob->GetBufferPointer(),
		pPixelShaderBlob->GetBufferSize(),
		nullptr,
		pPixelShader.ReleaseAndGetAddressOf()));
#ifdef MY_DEBUG
	if (GetDebug(gfx) != nullptr)
	{
		gfx.SetDebugName(pPixelShader.Get(), path.c_str());
	}
#endif

}

void PixelShader::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, numOfClassInstances);
}

