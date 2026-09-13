#include "pch.h"
#include "Timer.h"

namespace CommonUtilities
{
	Timer::Timer(float aFixedTickTime) :
		myLastTime(),
		myFixedTickTime(aFixedTickTime),
		myDeltaTime(0.0f),
		myAccumulatedTime(0.0f),
		myTotalTime(0.0),
		myFixedRateDelta(0),
		myFixedRate(0)
	{
		myStartTime = clock::now();
		myLastTime = myStartTime;
		myTotalTime = 0;
	}

	void Timer::Start()
	{
		myStartTime = clock::now();
		myLastTime = myStartTime;
		myTotalTime = 0;
	}

	void Timer::Update()
	{
		auto currentTime = clock::now();
		std::chrono::duration<float> delta = currentTime - myLastTime;
		std::chrono::duration<double> total = currentTime - myStartTime;
		myLastTime = currentTime;

		myDeltaTime = delta.count();
		myTotalTime = total.count();

		myAccumulatedTime += myDeltaTime;
		myFixedRateDelta = 0;

		while (myAccumulatedTime >= myFixedTickTime)
		{
			myAccumulatedTime -= myFixedTickTime;
			++myFixedRateDelta;
			++myFixedRate;
		}
	}
}

CommonUtilities::Timer globalTimer;
