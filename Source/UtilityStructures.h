#pragma once
#include "LightHelper.h"

struct vbPosNormalTex
{
	vbPosNormalTex() : pos(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), tex(0.0f, 0.0f) {};
	vbPosNormalTex(float posX, float posY, float posZ, float normX, float normY, float normZ, float u, float v)
		: pos(posX, posY, posZ), normal(normX, normY, normZ), tex(u, v) {}
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 tex;
};

struct vbPosNormalTexTangent
{
	vbPosNormalTexTangent() : pos(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), tex(0.0f, 0.0f), tangent(0.0f, 0.0f, 0.0f) {};
	vbPosNormalTexTangent(float posX, float posY, float posZ, float normX, float normY, float normZ, float u, float v,
		float tX, float tY, float tZ)
		: pos(posX, posY, posZ), normal(normX, normY, normZ), tex(u, v), tangent(tX, tY, tZ) {}
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 tex;
	DirectX::XMFLOAT3 tangent;
};

struct vbSkinnedVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 tex;
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT3 BoneWeights;
	BYTE BoneIndices[4];
};

struct cbShadowMap
{
	cbShadowMap() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMMATRIX lightWVP;
	DirectX::XMMATRIX texTransform;
};

struct cbCreateNormalMap
{
	cbCreateNormalMap() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMMATRIX worldView;
	DirectX::XMMATRIX worldInvTransposeView;
	DirectX::XMMATRIX worldViewProjection;
};

struct cbDefaultMatricesVS
{
	cbDefaultMatricesVS() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX viewProjection;
	DirectX::XMMATRIX worldInvTranspose;
	DirectX::XMMATRIX texTransform;
	DirectX::XMMATRIX shadowTransform;
	DirectX::XMMATRIX matTransform;
	DirectX::XMMATRIX worldViewProjTex;
	DirectX::XMFLOAT3 cameraPosition;
	int pad0;
	BOOL enableDisplacementMapping = false;
};

struct cbComputeSSAO
{
	DirectX::XMMATRIX viewToTexSpace;
};

struct cbComputeSSAOconstData
{
	DirectX::XMVECTOR offsetVectors[14];
	DirectX::XMFLOAT4 frustumFarCorners[4];

	const float gOcclusionRadius = 0.5f;
	const float gOcclusionFadeStart = 0.2f;
	const float gOcclusionFadeEnd = 2.0f;
	const float gSurfaceEpsilon = 0.05f;
};

struct cbBlurSSAO
{
	float texelWidth;
	float texelHeight;
	BOOL horizBool;
	int pad0;
};

struct cbDefaultLightPSdata
{
	DirectionalLightEx dirLight;
	DirectX::XMFLOAT4 fogColor = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	DirectX::XMFLOAT4 ambientLight = DirectX::XMFLOAT4(0.25f, 0.25f, 0.35f, 1.0f);
	float fogstart = 50.0f;
	float fogRange = 200.0f;
	int pad2;
	int pad3;

};

struct cbDefaultLightPSPerFrame
{
	MaterialEx mat;
	DirectX::XMFLOAT3 camPositon;
	BOOL disableTexSampling;
	DirectX::XMFLOAT3 lightDirection;
	BOOL useSSAO;
	BOOL alphaClip;
	int pad0;
	int pad1;
	int pad2;
};


struct cbGPUWavesVSConstData
{
	cbGPUWavesVSConstData() { ZeroMemory(this, sizeof(this)); }
	float spatialStep;
	float displacementMapTexelSize[2];
	float padding;
};

struct cbVSTesselationWaves
{
	cbVSTesselationWaves() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX worldViewProjection;
	DirectX::XMMATRIX worldInvTranspose;
	DirectX::XMMATRIX texTransform;
	DirectX::XMMATRIX waveDispTexTransform0;
	DirectX::XMMATRIX waveDispTexTransform1;
	DirectX::XMMATRIX waveNormalTexTransform0;
	DirectX::XMMATRIX waveNormalTexTransform1;
	DirectX::XMFLOAT3 cameraPosition;
};

struct CBufferNames
{
	const std::string normalMap = "NormalMap";
	const std::string shadowMap = "ShadowMap";
	const std::string defaultVS = "defaultVS";
	const std::string ssaoBlur = "ssaoBlur";
	const std::string ssaoPerFrame = "ssaoPerFrame";
	//init at runtime
	const std::string ssaoConstData = "ssaoConstData";
	const std::string defaultLightPerFrame = "defaultLightPerFrame";
	const std::string defaultLightData = "defaultLightData";
	const std::string tessWavesMatrices = "tessWavesMatrices";
	const std::string terrainHSPlainsData = "terrainHullShaderPlanes";
	const std::string terrainTexelInfo = "terrainTexelInfo";
	const std::string particleStreamOutGS = "particleStreamOutGS";
	const std::string computeWavesVSData = "computeWavesVSInitData";
	const std::string computeWavesCSPerFrame = "computWavesCSPerFrame";
	const std::string skinnedMeshBoneTransforms = "skinnedMeshBonesTransforms";
	
};

struct vbPosTexBoundsY
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 tex;
	DirectX::XMFLOAT2 boundsY;
};


struct cbHSTerrainPerFrame
{
	cbHSTerrainPerFrame() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMFLOAT4 worldFrustumPlanes[6];
	DirectX::XMFLOAT3 cameraPosition;
	float padding = 0.0f;


};

struct cbPSerrainTexel
{
	cbPSerrainTexel() { ZeroMemory(this, sizeof(this)); }
	float texelCellSpaceU;
	float texelCellSpaceV;
	float worldCellSpace;
	float padding;
};

struct Particle
{
	DirectX::XMFLOAT3 initialPos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 initialVel = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT2 size = DirectX::XMFLOAT2(0.0f, 0.0f);
	float age = 0.0f;
	unsigned int type = 0;
};
struct cbParticleStreamOutGS
{
	cbParticleStreamOutGS() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMFLOAT3  emitterPositon;
	float timeStep;
	float gameTime;
	float padding[3] = { 0.0f, 0.0f, 0.0f };
};

struct cbWavesUpdateCS
{
	cbWavesUpdateCS() { ZeroMemory(this, sizeof(this)); }
	float waveConstant0;
	float waveConstant1;
	float waveConstant2;
	float disturbMagnitute;

	int disturbIndex[2];
	int padding[2];
};

struct Subset
{
	UINT ID = -1;
	UINT VertexStart = 0;
	UINT VertexCount = 0;
	UINT FaceStart = 0;
	UINT FaceCount = 0;
};

struct M3dMaterial
{
	std::string name;
	DirectX::XMFLOAT4 diffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT3 fresnelR0 = { 0.01f, 0.01f, 0.01f };
	float shininess = 0.2f;
	bool alphaClip = false;

	std::string materialTypeName;
	std::wstring diffuseMapName;
	std::wstring normalMapName;
};

struct cbSkinnedMesh
{
	DirectX::XMFLOAT4X4 boneTransforms[96];
};


struct M3dModelNames
{
	const std::string templeBase = "templeBase";
	const std::string pillar1 = "Pillar1";
	const std::string pillar2 = "Pillar2";
	const std::string pillar3 = "Pillar3";
	const std::string pillar4 = "Pillar4";
	const std::string rock = "rock";
	const std::string stairs = "stairs";
	const std::string tree = "tree";
	const std::string box = "box";
	const std::string soldier = "soldier";
	const std::string backPack = "backPack";

};

struct TempleDemoWorlds
{
	TempleDemoWorlds()
	{
		templebase.push_back(DirectX::XMMatrixIdentity());
		templebase.push_back(DirectX::XMMatrixIdentity());
		templebase.push_back(DirectX::XMMatrixIdentity());
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(0.8f, 0.8f, 0.8f);
		DirectX::XMMATRIX offset = DirectX::XMMatrixTranslation(-5.0f, 1.5f, 5.0f);
		DirectX::XMMATRIX rotation;
		pillar1.push_back(scale * offset);
		offset = DirectX::XMMatrixTranslation(5.0f, 1.5f, 5.0f);
		pillar2.push_back(scale * offset);
		offset = DirectX::XMMatrixTranslation(5.0f, 1.5f, -5.0f);
		pillar3.push_back(scale * offset);
		scale = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
		offset = DirectX::XMMatrixTranslation(-5.0f, 1.0f, -5.0f);
		pillar4.push_back(scale * offset);
		scale = DirectX::XMMatrixScaling(0.8f, 0.8f, 0.8f);
		offset = DirectX::XMMatrixTranslation(-1.0f, 1.4f, -7.0f);
		rock.push_back(scale * offset);
		offset = DirectX::XMMatrixTranslation(5.0f, 1.2f, -2.0f);
		rock.push_back(scale * offset);
		offset = DirectX::XMMatrixTranslation(-4.0f, 1.3f, 3.0f);
		rock.push_back(scale * offset);
		offset = DirectX::XMMatrixTranslation(0.0f, -2.5f, -12.0f);
		rotation = DirectX::XMMatrixRotationY(0.5f * DirectX::XM_PI);
		stairs.push_back(rotation * offset);
		offset = DirectX::XMMatrixTranslation(0.0f, 1.3f, 0.0f);
		tree.push_back(offset);
		offset = DirectX::XMMatrixTranslation(0.0f, 4.0f, 6.0f);
		box.push_back(offset);
	}
	std::vector<DirectX::XMMATRIX> templebase;
	std::vector<DirectX::XMMATRIX> pillar1;
	std::vector<DirectX::XMMATRIX> pillar2;
	std::vector<DirectX::XMMATRIX> pillar3;
	std::vector<DirectX::XMMATRIX> pillar4;
	std::vector<DirectX::XMMATRIX> rock;
	std::vector<DirectX::XMMATRIX> stairs;
	std::vector<DirectX::XMMATRIX> tree;
	std::vector<DirectX::XMMATRIX> box;

};

enum class Technique
{
	NormalMap,
	DefaultLight,
	ShadowMap,

};


enum class ShaderPicker
{
	UpdateWaves_CS,
	DisturbWaves_CS,
	Sky_VS_PS,
	DisplacementWaves_VS_HS_DS_PS,
	TerrainHeightMap,
	Particles_FireStreamOut_VS_GS,
	Particles_FireDraw_VS_GS_PS,
	Particles_RainStreamOut_VS_GS,
	Particles_RainDraw_VS_GS_PS,
	Particles_ExplosionStreamOut_VS_GS,
	Particle_ExplosionDraw_VS_GS_PS,
	Particle_FountainStreamOut_VS_GS,
	Particle_FountainDraw_VS_GS_PS,
	ComputeSSAO_VS_PS,
	DrawDebugTexQuad_VS_PS,
	SSAOBlur_VS_PS,
	NormalMap_VS_PS, 
	ShadowMap_VS_PS,
	DefaultLight_VS_PS,
	ComputeWaves_VS_PS_CS,
	SkinnedModelNormalMap_VS,
	SkinnedModelShadowMap_VS,
	SkinnedModelDefaultLight_VS,
};

enum class ParticlePick
{
	Fire,
	Rain,
	Explosion,
	Fountain
};