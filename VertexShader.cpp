#include "VertexShader.h"

VertexShader::VertexShader(Graphics& gfx, const std::wstring& path)
{
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), pVertexShaderBlob.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(GetDevice(gfx)->CreateVertexShader(
		pVertexShaderBlob->GetBufferPointer(),
		pVertexShaderBlob->GetBufferSize(),
		nullptr,
		pVertexShader.ReleaseAndGetAddressOf()));
#ifdef MY_DEBUG
	if (path != std::wstring())
	{
		gfx.SetDebugName(pVertexShader.Get(), path.c_str());
	}
#endif
}

void VertexShader::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, numOfClassInstances);
}

ID3DBlob* VertexShader::GetByteCode() const noexcept
{
	return pVertexShaderBlob.Get();
}
