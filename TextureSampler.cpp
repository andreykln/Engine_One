#include "TextureSampler.h"

TextureSampler::TextureSampler(Graphics& gfx, ShaderType type)
	: sType(type)
{
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; //D3D11_FILTER_MIN_MAG_MIP_LINEAR D3D11_FILTER_ANISOTROPIC
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;

	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	DX::ThrowIfFailed(GetDevice(gfx)->CreateSamplerState(&samplerDesc, &pSamplerState));
}

void TextureSampler::Bind(Graphics& gfx) noexcept
{
	switch (sType)
	{
	case ShaderType::Pixel:
	{
		GetContext(gfx)->PSSetSamplers(0u, 1u, &pSamplerState);
	}
		break;
	case ShaderType::Vertex:
	{
		GetContext(gfx)->VSSetSamplers(0u, 1u, &pSamplerState);
	}
		break;
	case ShaderType::Compute:
		break;
	case ShaderType::Domain:
	{
		GetContext(gfx)->DSSetSamplers(0u, 1u, &pSamplerState);
	}
		break;
	case ShaderType::Hull:
		break;
	case ShaderType::Geometry:
		break;
	default:
		break;
	}


}
