#pragma once
#include "WindowsEdited.h"




class Timer
{
public:
	Timer();
	Timer& operator=(const Timer&) = delete;
	Timer(const Timer&) = delete;

	float TotalTime() const;
	void Tick() ;

private:
	double secondsPerCount = {};
	double deltaTime = {};
	__int64 currentTime = {};
	__int64 baseTime = {};
	__int64 previousTime = {};
};

