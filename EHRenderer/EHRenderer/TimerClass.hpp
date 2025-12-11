#pragma once;

#include <windows.h>

using namespace std;

class TimerClass {
public:
	TimerClass();
	TimerClass(const TimerClass&);
	~TimerClass();

	bool Initialize();
	void Frame();

	float GetTime();

private:
	float _frequency;
	INT64 _startTime;
	float _frameTime;

};