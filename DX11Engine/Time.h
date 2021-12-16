#pragma once

class TimerHandle // TICK BASED TIMERS INSTEAD OF ACTUAL TIME
{

public:
	//TimerHandle(float duration, )
	//{
	//	
	//}
};

class Time
{
	Time() = default;
	~Time() { delete inst; }

	static Time* inst;

	float deltaTime = 0.f;

public:
	static Time* GetInst() { return inst == nullptr ? inst = new Time() : inst; }
	static float GetDeltaTime() { return GetInst()->deltaTime; }

	float Tick();
};