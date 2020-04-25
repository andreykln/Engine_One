#include "WaveSurface.h"

WaveSurface::WaveSurface(Graphics& gfx, float radius, UINT numSubdivisions)
{
	//AddBind(std::make_unique<VertexBuffer>(gfx, wave.currSolution, L"Waves"));
}

DirectX::XMMATRIX WaveSurface::GetTransform() const noexcept
{
	return m_Matrix * m_Centered;
}

void WaveSurface::Update(float dt) noexcept
{
	alpha = dt;
}

void WaveSurface::UpdateScene(float totalTime, float dt, Graphics& gfx)
{
	// every quarter second, generate a random wave
	static float t_base{};
	if ((totalTime - t_base) >= 0.25f)
	{
		t_base += 0.25;

		unsigned long i = 5 + 
	}
}
