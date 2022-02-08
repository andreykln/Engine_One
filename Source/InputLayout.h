#pragma once
// #include "Bindable.h"
struct InputLayout 
{
	////// MAIN

	static const UINT nPosNormalTexture = 3;
	const D3D11_INPUT_ELEMENT_DESC posNormalTexture[nPosNormalTexture] =
	{
		{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"Normal", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, sizeof(DirectX::XMFLOAT3),
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"TexCoordinate", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, sizeof(DirectX::XMFLOAT3) * 2,
		D3D11_INPUT_PER_VERTEX_DATA, 0u}
	};

	static const UINT nPosNormalTexcTangent = 4;
	const D3D11_INPUT_ELEMENT_DESC posNormalTexcTangent[nPosNormalTexcTangent] =
	{
		{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"Normal", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"TexCoord", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"Tangent", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u}
	};
	static const UINT nPosNormalTexcTangentSkinned = 6;
	const D3D11_INPUT_ELEMENT_DESC posNormalTexcTangentSkinned[nPosNormalTexcTangentSkinned] =
	{
		{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"Normal", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"TexCoord", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"Tangent", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"BoneWeights", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"BoneIndices", 0u, DXGI_FORMAT_R8G8B8A8_UINT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
	};

	static const UINT nTerrainHeightMap = 3;
	const D3D11_INPUT_ELEMENT_DESC terrainHeightMap[nTerrainHeightMap] =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"TEXCOORD", 1u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
	};

	static const UINT nParticleStreamOut = 5;
	const D3D11_INPUT_ELEMENT_DESC particleStreamOutIL[nParticleStreamOut] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"TEXCOORD", 1u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"TEXCOORD", 2u, DXGI_FORMAT_R32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"TEXCOORD", 3u, DXGI_FORMAT_R32_UINT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
	};

	static const UINT nParticleDraw = 5;
	const D3D11_INPUT_ELEMENT_DESC particleDrawIL[nParticleDraw] =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Velocity", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"Size", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"Age", 0u, DXGI_FORMAT_R32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"Type", 0u, DXGI_FORMAT_R32_UINT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
	};


	static const UINT nPosition = 1;
	const D3D11_INPUT_ELEMENT_DESC positonIL[nPosition] =
	{
		{ "Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u,
		D3D11_INPUT_PER_VERTEX_DATA, 0u }
	};
};

struct Stream_Out
{
	const UINT fireSize = 5;
	const D3D11_SO_DECLARATION_ENTRY fire[5] =
	{
		//Stream, semantic name, semantic index, start component, component count, output slot
		{ 0, "POSITION", 0, 0, 3, 0 }, //Position
		{ 0, "TEXCOORD", 0, 0, 3, 0 }, //Velocity
		{ 0, "TEXCOORD", 1, 0, 2, 0 }, //size
		{ 0, "TEXCOORD", 2, 0, 1, 0 }, //age
		{ 0, "TEXCOORD", 3, 0, 1, 0 }, //type
	};
};