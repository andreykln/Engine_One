#pragma once
#include "Bindable.h"
struct InputLayout 
{
	static const UINT nLightTextureElements = 3;
	const D3D11_INPUT_ELEMENT_DESC lightTexture[nLightTextureElements] =
	{
		{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"Normal", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, sizeof(DirectX::XMFLOAT3),
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"TexCoordinate", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, sizeof(DirectX::XMFLOAT3) * 2,
		D3D11_INPUT_PER_VERTEX_DATA, 0u}
	};
	

	static const UINT nlightTextureNormalMapping = 4;
	const D3D11_INPUT_ELEMENT_DESC lightTextureNormalMapping[nlightTextureNormalMapping] =
	{
		{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"Normal", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"TexCoordinate", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"Tangent", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u}
	};

	static const UINT nLightElements = 2;
	const D3D11_INPUT_ELEMENT_DESC light[nLightElements] =
	{
		{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"Normal", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u}
	};

	static const UINT nPosition_Color = 2;
	const D3D11_INPUT_ELEMENT_DESC positionColorIL[nPosition_Color] =
	{
		{ "Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u,
		D3D11_INPUT_PER_VERTEX_DATA, 0u },
		{ "Color", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 0u, sizeof(DirectX::XMFLOAT3),
		D3D11_INPUT_PER_VERTEX_DATA, 0u },
	};

	static const UINT nPositon = 1;
	const D3D11_INPUT_ELEMENT_DESC positonIL[nPositon] =
	{
		{ "Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u,
		D3D11_INPUT_PER_VERTEX_DATA, 0u }
	};

	static const UINT nTreeBillboardElements = 2;
	const D3D11_INPUT_ELEMENT_DESC treeBillboardIL[nTreeBillboardElements] =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Size", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	static const UINT nInstancedSkull = 7;
	const D3D11_INPUT_ELEMENT_DESC instancedSkull[nInstancedSkull] =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Normal", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, sizeof(DirectX::XMFLOAT3),
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"World", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,
		D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"World", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16u,
		D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"World", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32u,
		D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"World", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48u,
		D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"Color", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64u,
		D3D11_INPUT_PER_INSTANCE_DATA, 1},
	};

	static const UINT nTerrainHeightMap = 3;
	const D3D11_INPUT_ELEMENT_DESC terrainHeightMap[nInstancedSkull] =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"TEXCOORD", 1u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
	};

	static const UINT nParticle = 5;
	const D3D11_INPUT_ELEMENT_DESC particle[nParticle] =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Velocity", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"Size", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"Age", 0u, DXGI_FORMAT_R32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"Type", 0u, DXGI_FORMAT_R32_UINT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0u},
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