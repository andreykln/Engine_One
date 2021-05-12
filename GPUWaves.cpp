#include "GPUWaves.h"

GPUWaves::GPUWaves()
{
}

void GPUWaves::Initialize(UINT inRows, UINT inColumns, float dx, float dt, float speed, float damping)
{
	numRows = inRows;
	numColumns = inColumns;

	assert((numRows * numColumns) % 256 == 0);

	vertexCount = numRows * numColumns;
	triangleCount = (numRows - 1) * (numColumns - 1) * 2;

	timeStep = dt;
	spatialStep = dx;

	float d = damping * dt + 2.0f;
	float e = (speed * speed) * (dt * dt) / (dx * dx);

	waveConstant[0] = (damping * dt - 2.0f) / d;
	waveConstant[1] = (4.0f - 8.0f * e) / d;
	waveConstant[2] = (2.0f * e) / d;
}

UINT GPUWaves::GetRowCount() const
{
	return numRows;
}

UINT GPUWaves::GetColumnCount() const
{
	return numColumns;
}

UINT GPUWaves::GetVertexCount() const
{
	return vertexCount;
}

UINT GPUWaves::GetTriangleCount() const
{
	return triangleCount;
}

float GPUWaves::Width() const
{
	return numColumns * spatialStep;
}

float GPUWaves::Depth() const
{
	return numRows * spatialStep;
}

float GPUWaves::SpatialStep() const
{
	return spatialStep;
}
