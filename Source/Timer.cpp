#include "Timer.h"

Timer::Timer()
	: secondsPerCount(0), deltaTime(-1.0), currentTime(0), baseTime(0)
{
	LARGE_INTEGER freqMS = {};
	QueryPerformanceFrequency(&freqMS);
	secondsPerCount = 1.0 / freqMS.LowPart; 
	millisecondsPerCount = static_cast<double>(freqMS.QuadPart / 1000.0f);
	
}

float Timer::TotalTime() const 
{
	float t = static_cast<float>((currentTime - baseTime) * secondsPerCount);
	return t;
}

float Timer::Milliseconds() const
{
	float t = static_cast<float>((counterMSStart - baseTime) / millisecondsPerCount);
	return t;
}

float Timer::DeltaTime() const
{
	return static_cast<float>(deltaTime);
}

void Timer::Tick() 
{

	if (stopped)
	{
		deltaTime = 0.0;
		return;
	}
	QueryPerformanceCounter(&q_CurrentTime);
	currentTime = q_CurrentTime.QuadPart;

	//time difference between this frame and the previous
	deltaTime = (currentTime - previousTime) * secondsPerCount;

	//next frame
	previousTime = currentTime;

	//force nonnegative delta
	if (deltaTime < 0.0f) { deltaTime = 0.0; }

	//milliseconds
	counterMSStart = q_CurrentTime.QuadPart;
}

void Timer::Reset()
{
	__int64 t_currtime = {0};
	QueryPerformanceCounter((LARGE_INTEGER*)&t_currtime);

	baseTime = t_currtime;
	previousTime = t_currtime;
}

