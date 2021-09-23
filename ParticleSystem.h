#pragma once

#include "Shape.h"

class ParticleSystem : public Shape
{
	struct Particle
	{
		DirectX::XMFLOAT3 initialPos;
		DirectX::XMFLOAT3 initialVel;
		DirectX::XMFLOAT2 size;
		float age;
		unsigned int type;
	};

public:
	ParticleSystem(Graphics& gfx, UINT maxParticles);

	//time elapsed since the system was reset
	float GetAge() const;

	void SetCamPos(const DirectX::XMFLOAT3& camPosW);
	void SetEmitPos(const DirectX::XMFLOAT3& emitPosW);
	void SetEmitDir(const DirectX::XMFLOAT3& emitDirW);

	void Init(Graphics& gfx, ID3D11ShaderResourceView* texArraySRV, ID3D11ShaderResourceView* randomTexSRV);

	void Reset();
	void Update(float dt, float gameTime);
	void Draw(Graphics& gfx);

private:
	void BindToSOStage(Graphics& gfx);
	void UnbindFromSOStage(Graphics& gfx);
	ID3D11ShaderResourceView* CreateRandomTexture1DSRV(Graphics& gfx);

	void BuildVB(Graphics& gfx);


	UINT maxParticles;
	bool firstRun;

	float gameTime;
	float timeStep;
	float age;

	DirectX::XMFLOAT3 camPosW;
	DirectX::XMFLOAT3 emitPosW;
	DirectX::XMFLOAT3 emitDirW;

	ID3D11Buffer* pInitVB;
	ID3D11Buffer* pDrawVB;
	ID3D11Buffer* pStreamOutVB;

	ID3D11ShaderResourceView* texArraySRV = nullptr;
	ID3D11ShaderResourceView* randomTexSRV = nullptr;

};