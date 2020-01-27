#include "Timer.h"

Timer::Timer()
	: secondsPerCount(0), deltaTime(-1.0), currentTime(0), baseTime(0)
{
	__int64 countPerSecond = {};
	QueryPerformanceFrequency((LARGE_INTEGER*)(&countPerSecond));
	secondsPerCount = 1.0 / static_cast<double>(countPerSecond);
}

float Timer::TotalTime() const 
{
	float t = static_cast<float>((currentTime - baseTime) * secondsPerCount);
	return t;
}

void Timer::Tick() 
{
	__int64 t_currentTime = {};
	QueryPerformanceCounter((LARGE_INTEGER*)(&t_currentTime)); //TODO figure out why this throws error if I pass by value
	currentTime = t_currentTime;

	//time difference between this frame and the previous
	deltaTime = (currentTime - previousTime) * secondsPerCount;

	//next frame
	previousTime = currentTime;

	//force nonnegative delta
	if (deltaTime < 0.0f) { deltaTime = 0.0; }
}

