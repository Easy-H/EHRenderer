#include "TimerClass.hpp"

TimerClass::TimerClass()
{
}

TimerClass::TimerClass(const TimerClass&)
{
}

TimerClass::~TimerClass()
{
}

bool TimerClass::Initialize() {

	INT64 frequency = 0;

	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

	if (frequency == 0) return false;

	_frequency = frequency;

	QueryPerformanceCounter((LARGE_INTEGER*)&_startTime);

	return true;
}

void TimerClass::Frame() {

	INT64 currentTime = 0;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	INT64 elapsedTicks = currentTime - _startTime;
	_frameTime = (float)elapsedTicks / _frequency;

	_startTime = currentTime;


}

float TimerClass::GetTime()
{
	return _frameTime;
}
