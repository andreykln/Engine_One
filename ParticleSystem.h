#pragma once

#include "Shape.h"

class ParticleSystem : public Shape
{
	struct Particle
	{
		DirectX::XMFLOAT3 initialPos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		DirectX::XMFLOAT3 initialVel = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		DirectX::XMFLOAT2 size = DirectX::XMFLOAT2(0.0f, 0.0f);
		float age = 0.0f;
		unsigned int type = 0;
	};

public:
	ParticleSystem(Graphics& gfx, UINT maxParticles);
	void DrawFire(Graphics& gfx, Shaders* pShaders, DirectX::XMMATRIX viewProjection, DirectX::XMFLOAT3 cameraPos,
		DirectX::XMFLOAT3 emitPos, float timeStep, float gameTime);


	bool mFirstRun = true;
private:
	void UpdateStreamOutConstBuffer(Graphics& gfx, DirectX::XMFLOAT3 emitPos, float timeStep, float gameTime);
	void BindToSOStage(Graphics& gfx);
	void UnbindFromSOStage(Graphics& gfx);
	void UpdateParticleDrawConstBuffer(Graphics& gfx, DirectX::XMMATRIX viewProjection, DirectX::XMFLOAT3 cameraPos);

	ID3D11ShaderResourceView* CreateRandomTexture1DSRV(Graphics& gfx);



	UINT maxParticles;
	bool firstRun = true;

	float gameTime;
	float timeStep;
	float age;
	int frameCounter = 0;

	DirectX::XMFLOAT3 camPosW;
	DirectX::XMFLOAT3 emitPosW;
	DirectX::XMFLOAT3 emitDirW;

	ID3D11Buffer* pInitVB;
	ID3D11Buffer* pDrawVB;
	ID3D11Buffer* pStreamOutVB;

	ID3D11SamplerState* pSSGSSO = nullptr;
	ID3D11SamplerState* pSSDrawPixel = nullptr;

	//constant buffers
	ID3D11Buffer* pGSSOParticleFire = nullptr;
	CB_GS_StreamOut GSSOparticleFireData;

	ID3D11Buffer* pGSParticleDraw = nullptr;
	CB_CameraPosition_ViewProj camPosVP;

	ID3D11ShaderResourceView* texArraySRV = nullptr;
	ID3D11ShaderResourceView* randomTexSRV = nullptr;
	ID3D11ShaderResourceView* psFireDrawTexture = nullptr;
	ID3D11ShaderResourceView* psRainDropTexture = nullptr;

};