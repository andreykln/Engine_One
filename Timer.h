#pragma once
#include "WindowsEdited.h"




class Timer
{
public:
	Timer();
	Timer& operator=(const Timer&) = delete;
	Timer(const Timer&) = delete;

	float TotalTime() const;
	//don't use it in SetTitle, SetTitle is quite expensive function
	float Milliseconds() const;
	float DeltaTime() const;
	void Tick() ;
	void Reset();
private:
	LARGE_INTEGER q_CurrentTime{};
	bool stopped = false;
	double secondsPerCount = {};
	double millisecondsPerCount = {};
	double deltaTime = {};
	__int64 currentTime = {};
	__int64 baseTime = {};
	__int64 previousTime = {};
	__int64 counterMSStart = {};
};

