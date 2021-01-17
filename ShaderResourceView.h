#pragma once
#include "Bindable.h"
#include "DirectXTex/DirectXTex/DirectXTexP.h"
#include "DirectXTex/DDSTextureLoader/DDSTextureLoader11.h"

class ShaderResourceView : public Bindable
{
public:
	//create SRV for each texture
	ShaderResourceView(Graphics& gfx, std::wstring* in_path, UINT in_NumSRVs);
	//create texture array
	ShaderResourceView(Graphics& gfx, std::wstring* in_path, UINT in_NumofTextures, bool texarr);
	void Bind(Graphics& gfx) noexcept override;
	~ShaderResourceView()
	{
		delete[] pSRVArray;
	}
private:
	UINT numSRVs{};
	std::wstring* path;
	std::wstring* arrPath;
	UINT index{};
	UINT numTextures{};
	DirectX::TexMetadata textureMetaData;
	ID3D11Resource* pResource = nullptr;
	ID3D11Texture2D* pTexture = nullptr;
	std::vector <ID3D11Texture2D*> textureArray;
	ID3D11Texture2D* textureCarray[60];
	//ID3D11ShaderResourceView* pShaderResourceView = nullptr;
	ID3D11ShaderResourceView** pSRVArray = new ID3D11ShaderResourceView*[numSRVs];
	ID3D11ShaderResourceView* pSRVTexArray = nullptr;

	ID3D11SamplerState* pSamplerState = nullptr;


};

