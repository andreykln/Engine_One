#pragma once
#include "Bindable.h"
// #include "DirectXTex/DirectXTex/DirectXTexP.h"
// #include "DirectXTex/DDSTextureLoader/DDSTextureLoader11.h"

class ShaderResourceView : public Bindable
{
public:
	//create SRV for each texture
	ShaderResourceView(Graphics& gfx, std::wstring* in_path, UINT in_startSlot, UINT in_NumSRVs, ShaderType target);
	//return SRV ptr to bind inside of a Drawable
	ShaderResourceView(Graphics& gfx, const wchar_t* path);
	ID3D11ShaderResourceView* GetSRV() const;
	//create texture array
	ShaderResourceView(std::wstring* in_path, UINT in_NumofTextures);
	ID3D11ShaderResourceView* GetTextureArray(Graphics& gfx);
	static ID3D11ShaderResourceView* CreateCubeMap(Graphics& gfx, std::wstring* in_path);

	void Bind(Graphics& gfx) noexcept override;
	~ShaderResourceView()
	{
		delete[] pSRVArray;
	}
private:
	ShaderType SRVtarget;
	UINT numSRVs{};
	std::wstring* path;
	std::wstring* arrPath;
	UINT index{};
	UINT numTextures{};
	UINT startSlot = 0;
	bool textureArray = false;
	DirectX::TexMetadata textureMetaData;
	std::vector<DirectX::Image> ImagesArray;
	ID3D11Resource* pResource = nullptr;
	ID3D11Texture2D* pTexture = nullptr;
	//ID3D11ShaderResourceView* pShaderResourceView = nullptr;
	ID3D11ShaderResourceView** pSRVArray = new ID3D11ShaderResourceView*[numSRVs];
	ID3D11ShaderResourceView* pSRVTexArray = nullptr;
	ID3D11ShaderResourceView* pSRVReturn = nullptr;

	ID3D11SamplerState* pSamplerState = nullptr;


};

