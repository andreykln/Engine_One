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
	float Width() const;
	float Depth() const;
	float GetTimeStep() const;
	//return the solution at the i'th grid point
	const DirectX::XMFLOAT3& operator[] (int i) const { return currSolution[i]; }

	//returns the solution normal at the ith grid point
	const DirectX::XMFLOAT3& Normal(int i) { return normal[i]; }

	//returns the unit tangent vector at the ith grid point in the local x-axis
	const DirectX::XMFLOAT3& Tangent(int i) { return tangent[i]; }

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

	DirectX::XMFLOAT3* prevSolution;
	DirectX::XMFLOAT3* currSolution;
	DirectX::XMFLOAT3* normal;
	DirectX::XMFLOAT3* tangent;
	float K1{};
	float K2{};
	float K3{};

};

