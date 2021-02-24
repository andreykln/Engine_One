#include "GeometryShader.h"

GeometryShader::GeometryShader(Graphics& gfx, const std::wstring& path)
{
#ifdef MY_DEBUG
	gfx.CheckFileExistence(gfx, path);
#endif // MY_DEBUG
	Microsoft::WRL::ComPtr<ID3DBlob> pGeometryShaderBlob;
	DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), pGeometryShaderBlob.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(GetDevice(gfx)->CreateGeometryShader(
		pGeometryShaderBlob->GetBufferPointer(),
		pGeometryShaderBlob->GetBufferSize(),
		nullptr,
		pGeometryShader.ReleaseAndGetAddressOf()));
#ifdef MY_DEBUG
	if (GetDebug(gfx) != nullptr)
	{
		gfx.SetDebugName(pGeometryShader.Get(), path.c_str());
	}
#endif
}

void GeometryShader::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->GSSetShader(pGeometryShader.Get(), nullptr, numClassInstances);
}
