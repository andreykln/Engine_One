#pragma once
#include "Bindable.h"
#include "DirectXTex/DirectXTex/DirectXTexP.h"
#include "DirectXTex/DDSTextureLoader/DDSTextureLoader11.h"

class ShaderResourceView : public Bindable
{
public:
	ShaderResourceView(Graphics& gfx, std::wstring* in_path, UINT in_NumSRVs);
	void Bind(Graphics& gfx) noexcept override;
	~ShaderResourceView()
	{
		delete[] pSRVArray;
	}
private:
	UINT numSRVs = 0u;
	std::wstring* path;
	UINT index = 0;
	DirectX::TexMetadata textureMetaData;
	ID3D11Resource* pResource = nullptr;
	ID3D11Texture2D* pTexture = nullptr;
	//ID3D11ShaderResourceView* pShaderResourceView = nullptr;
	ID3D11ShaderResourceView** pSRVArray = new ID3D11ShaderResourceView*[numSRVs];

	ID3D11SamplerState* pSamplerState = nullptr;
};

