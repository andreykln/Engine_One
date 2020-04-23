#include "Waves.h"

Waves::Waves()
	: numRows(0), numColumns(0), vertexCount(0), triangleCount(0),
	K1(0.0f), K2(0.0f), K3(0.0f), timeStep(0.0f), spatialStep(0.0f),
	prevSolution(0), currSolution(0)
{

}

Waves::~Waves()
{
	delete[] prevSolution;
	delete[] currSolution;
}

UINT Waves::GetRowCount() const
{
	return numRows;
}

UINT Waves::GetColumnCount() const
{
	return numColumns;
}

UINT Waves::GetVertexCount() const
{
	return vertexCount;
}

UINT Waves::GetTriangleCount() const
{
	return triangleCount;
}

void Waves::Init(UINT m, UINT n, float dx, float dt, float speed, float damping)
{
	numRows = m;
	numColumns = n;

	vertexCount = m * n;
	triangleCount = (m - 1) * (n - 1) * 2;

	timeStep = dt;
	spatialStep = dx;

	float d = damping * dt + 2.0f;
	float e = (speed * speed) * (dt * dt) / (dx * dx);
	K1 = (damping * dt - 2.0f) / d;
	K2 = (4.0f - 8.0f * e) / d;
	K3 = (2.0f * e) / d;

	// In case Init() called again.
	delete[] prevSolution;
	delete[] currSolution;

	prevSolution = new DirectX::XMFLOAT3[m * n];
	currSolution = new DirectX::XMFLOAT3[m * n];

	// Generate grid vertices in system memory.

	float halfWidth = (n - 1) * dx * 0.5f;
	float halfDepth = (m - 1) * dx * 0.5f;
	for (UINT i = 0; i < m; ++i)
	{
		float z = halfDepth - i * dx;
		for (UINT j = 0; j < n; ++j)
		{
			float x = -halfWidth + j * dx;

			prevSolution[i * n + j] = DirectX::XMFLOAT3(x, 0.0f, z);
			currSolution[i * n + j] = DirectX::XMFLOAT3(x, 0.0f, z);
		}
	}
}
