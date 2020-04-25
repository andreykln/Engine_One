#pragma once
#include "WindowsEdited.h"
#include <DirectXMath.h>
#include <cassert>
#include <algorithm>

class Waves
{
public:
	Waves();
	~Waves();

	UINT GetRowCount() const;
	UINT GetColumnCount() const;
	UINT GetVertexCount() const;
	UINT GetTriangleCount() const;
	float GetTimeStep() const;
	//return the solution at the i'th grid point
	const DirectX::XMFLOAT3& operator[] (int i) const { return currSolution[i]; }

	void Init(UINT m, UINT n, float dx, float dt, float speed, float damping);
	void UpdateSolution(float dt);
	void Disturb(UINT i, UINT j, float magnitude);
private:
	UINT numRows{};
	UINT numColumns{};
	UINT vertexCount{};
	UINT triangleCount{};
	// Simulation constants we can precompute.


	float timeStep;
	float spatialStep;
public:
	DirectX::XMFLOAT3* prevSolution;
	DirectX::XMFLOAT3* currSolution;
	float K1{};
	float K2{};
	float K3{};

};

