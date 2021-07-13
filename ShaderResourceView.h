#pragma once
#include "Bindable.h"
#include "DirectXTex/DirectXTex/DirectXTexP.h"
#include "DirectXTex/DDSTextureLoader/DDSTextureLoader11.h"

class ShaderResourceView : public Bindable
{
public:
	//create SRV for each texture
	ShaderResourceView(Graphics& gfx, std::wstring* in_path, UINT in_startSlot, UINT in_NumSRVs);
	//create texture array
	ShaderResourceView(Graphics& gfx, std::wstring* in_path, UINT in_NumofTextures, UINT in_NumSRVs, bool texarr);
	static ID3D11ShaderResourceView* CreateCubeMap(Graphics& gfx, std::wstring* in_path);
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
	UINT startSlot = 0;
	bool textureArray = false;
	DirectX::TexMetadata textureMetaData;
	ID3D11Resource* pResource = nullptr;
	ID3D11Texture2D* pTexture = nullptr;
	//ID3D11ShaderResourceView* pShaderResourceView = nullptr;
	ID3D11ShaderResourceView** pSRVArray = new ID3D11ShaderResourceView*[numSRVs];
	ID3D11ShaderResourceView* pSRVTexArray = nullptr;

	ID3D11SamplerState* pSamplerState = nullptr;


};

