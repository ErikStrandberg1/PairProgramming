#pragma once
#include <chrono>
#include <cstdint>

namespace CommonUtilities
{
	using clock = std::chrono::high_resolution_clock;
	using time_point = std::chrono::high_resolution_clock::time_point;


	class Timer
	{
	public:
		Timer(float aFixedTickTime = 1.f / 60.f);
		Timer(const Timer& aTimer) = delete;
		Timer& operator=(const Timer& aTimer) = delete;
		void Start();
		void Update();
		float GetDeltaTime() const { return myDeltaTime; }
		double GetTotalTime() const { return myTotalTime; }
		int GetFixedRateTickDeltaCount() const { return myFixedRateDelta; }
		uint64_t GetFixedRateTickCount() const { return myFixedRate; }

	private:
		time_point myStartTime;
		time_point myLastTime;
		float myFixedTickTime;
		float myDeltaTime;
		float myAccumulatedTime;
		double myTotalTime = 0;
		int myFixedRateDelta;
		uint64_t myFixedRate;
	};

	static Timer globalTimer;
}

extern CommonUtilities::Timer globalTimer;
