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
	delete[] normal;
	delete[] tangent;
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

float Waves::Width() const
{
	return numColumns * spatialStep;
}

float Waves::Depth() const
{
	return numRows * spatialStep;
}

float Waves::GetTimeStep() const
{
	return timeStep;
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
	delete[] normal;
	delete[] tangent;

	prevSolution = new DirectX::XMFLOAT3[(long long)m * n];
	currSolution = new DirectX::XMFLOAT3[(long long)m * n];
	normal = new DirectX::XMFLOAT3[(long long)m * n];
	tangent = new DirectX::XMFLOAT3[(long long)m * n];

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
			normal[i * n + j] = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
			tangent[i * n + j] = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
		}
	}
}

void Waves::UpdateSolution(float dt)
{
	static float t = 0;

	// Accumulate time.
	t += dt;

	// Only update the simulation at the specified time step.
	if (t >= timeStep)
	{
		// Only update interior points; we use zero boundary conditions.
		for (DWORD i = 1; i < numRows - 1; ++i)
		{
			for (DWORD j = 1; j < numColumns - 1; ++j)
			{
				// After this update we will be discarding the old previous
				// buffer, so overwrite that buffer with the new update.
				// Note how we can do this inplace (read/write to same element) 
				// because we won't need prev_ij again and the assignment happens last.

				// Note j indexes x and i indexes z: h(x_j, z_i, t_k)
				// Moreover, our +z axis goes "down"; this is just to 
				// keep consistent with our row indices going down.

				prevSolution[i * numColumns + j].y =
					K1 * prevSolution[i * numColumns + j].y +
					K2 * currSolution[i * numColumns + j].y +
					K3 * (currSolution[(i + 1) * numColumns + j].y +
						currSolution[(i - 1) * numColumns + j].y +
						currSolution[i * numColumns + j + 1].y +
						currSolution[i * numColumns + j - 1].y);
			}
		}

		// We just overwrote the previous buffer with the new data, so
		// this data needs to become the current solution and the old
		// current solution becomes the new previous solution.
		std::swap(prevSolution, currSolution);

		t = 0.0f; // reset time

		//compute normals using finite difference scheme
		for (UINT i = 1; i < numRows - 1; ++i)
		{
			for (UINT j = 1; j < numColumns; ++j)
			{
				float l = currSolution[i * numColumns + j - 1].y;
				float r = currSolution[i * numColumns + j + 1].y;
				float t = currSolution[(i - 1) * numColumns + j].y;
				float b = currSolution[(i + 1) * numColumns + j].y;
				normal[i * numColumns + j].x = -r + l;
				normal[i * numColumns + j].y = 2.0f * spatialStep;
				normal[i * numColumns + j].z = b - t;

				DirectX::XMVECTOR n = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&normal[i * numColumns + j]));
				DirectX::XMStoreFloat3(&normal[i * numColumns + j], n);

				tangent[i * numColumns + j] = DirectX::XMFLOAT3(2.0f * spatialStep, r - l, 0.0f);
				DirectX::XMVECTOR T = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&tangent[i * numColumns + j]));
				DirectX::XMStoreFloat3(&tangent[i * numColumns + j], T);
			}
		}
	}
}

void Waves::Disturb(UINT i, UINT j, float magnitude)
{
	// Don't disturb boundaries.
	assert(i > 1 && i < numRows - 2);
	assert(j > 1 && j < numColumns - 2);

	float halfMag = 0.5f * magnitude;

	// Disturb the ij'th vertex height and its neighbors.
	currSolution[i * numColumns + j].y += magnitude;
	currSolution[i * numColumns + j + 1].y += halfMag;
	currSolution[i * numColumns + j - 1].y += halfMag;
	currSolution[(i + 1) * numColumns + j].y += halfMag;
	currSolution[(i - 1) * numColumns + j].y += halfMag;
}
