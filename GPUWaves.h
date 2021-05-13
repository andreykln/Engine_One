#pragma once
#include "Shape.h"
#include "MathHelper.h"
#include <cassert>

class GPUWaves
{
public:
	GPUWaves();
	void Initialize(UINT inRows, UINT inColumns, float dx, float dt, float speed, float damping);
	UINT GetRowCount() const;
	UINT GetColumnCount() const;
	UINT GetVertexCount() const;
	UINT GetTriangleCount() const;
	float Width() const;
	float Depth() const;
	float SpatialStep() const;
	float TimeStep() const;
private:
	UINT numRows = 0u;
	UINT numColumns = 0u;

	UINT vertexCount = 0u;
	UINT triangleCount = 0u;


public:
	//simulation constants
	float waveConstant[3]{ 0.0f, 0.0f, 0.0f };
	float timeStep = 0.0f;
	float spatialStep = 0.0f;
private:
	ID3D11Resource* pPreviousSolution = nullptr;
	ID3D11Resource* pCurrentSolution = nullptr;
	ID3D11Resource* pNextSolution = nullptr;

	ID3D11Resource* pPrevUploadBuffer = nullptr;
	ID3D11Resource* pCurrentUploadBuffer = nullptr;
};