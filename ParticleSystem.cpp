#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(Graphics& gfx, UINT maxParticles)
	: maxParticles(maxParticles)
{
	//Vertex buffer for Stream-Out
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(Particle) * maxParticles;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
	vbd.CPUAccessFlags = 0u;
	vbd.MiscFlags = 0u;

	gfx.pgfx_pDevice->CreateBuffer(&vbd, 0u, &pStreamOutVB);

	randomTexSRV = CreateRandomTexture1DSRV(gfx);

	TextureSampler* pGSSOsampler = new TextureSampler(gfx, ShaderType::Geometry);
	AddBind(pGSSOsampler);
}

void ParticleSystem::BindToSOStage(Graphics& gfx)
{
	gfx.pgfx_pDeviceContext->SOSetTargets(1u, &pStreamOutVB, 0u);
	gfx.pgfx_pDeviceContext->GSSetShaderResources(0u, 1u, &randomTexSRV);
}

void ParticleSystem::UnbindFromSOStage(Graphics& gfx)
{
	ID3D11Buffer* bufferArray[1] = { 0 };
	gfx.pgfx_pDeviceContext->SOSetTargets(1u, bufferArray, 0u);

}

ID3D11ShaderResourceView* ParticleSystem::CreateRandomTexture1DSRV(Graphics& gfx)
{
	DirectX::XMFLOAT4 *randomValues = new DirectX::XMFLOAT4[1024];

	for (int i = 0; i < 1024; ++i)
	{
		randomValues[i].x = MathHelper::RandomFloatWithinRange(-1.0f, 1.0f);
		randomValues[i].y = MathHelper::RandomFloatWithinRange(-1.0f, 1.0f);
		randomValues[i].z = MathHelper::RandomFloatWithinRange(-1.0f, 1.0f);
		randomValues[i].w = MathHelper::RandomFloatWithinRange(-1.0f, 1.0f);
	}

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = randomValues;
	initData.SysMemPitch = 1024 * sizeof(DirectX::XMFLOAT4); //it suppose to have no meaning for 1D texture
	initData.SysMemSlicePitch = 0u;

	D3D11_TEXTURE1D_DESC texDesc;
	texDesc.Width = 1024;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1u;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0u;
	texDesc.MiscFlags = 0u;

	ID3D11Texture1D* pRandomTex = nullptr;
	gfx.pgfx_pDevice->CreateTexture1D(&texDesc, &initData, &pRandomTex);

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
	viewDesc.Texture1D.MostDetailedMip = 0;

	ID3D11ShaderResourceView* pSRV = nullptr;
	gfx.pgfx_pDevice->CreateShaderResourceView(pRandomTex, &viewDesc, &pSRV);

	delete[] randomValues;
	return pSRV;
}

