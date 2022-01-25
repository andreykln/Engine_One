#pragma once
#include "Shape.h"

class ParticleSystem
{
public:
	ParticleSystem(Graphics& gfx, UINT maxParticles);
	ID3D11Buffer* GetDrawVertexBuffer();
	ID3D11Buffer* GetStreamOutVertexBuffer();
	ID3D11ShaderResourceView* GetRandomTexSRV();
	ID3D11Buffer* GetInitVB();

	const std::wstring fireTex = L"flame";
	const std::wstring raindTex = L"raindrop";
private:
	ID3D11ShaderResourceView* CreateRandomTexture1DSRV(Graphics& gfx);
	UINT maxParticles;
	ID3D11Buffer* pInitVB;
	ID3D11Buffer* pDrawVB;
	ID3D11Buffer* pStreamOutVB;
	ID3D11ShaderResourceView* randomTexSRV = nullptr;
};