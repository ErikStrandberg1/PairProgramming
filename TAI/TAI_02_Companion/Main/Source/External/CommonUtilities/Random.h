#pragma once
#include <random>
#include <cstdint>

namespace CommonUtilities
{
	class RNG
	{
	public:
		explicit RNG(uint32_t seed = 5489u) : rng(seed)
		{
		}

		void Seed(uint32_t seed) { rng.seed(seed); }

		int RangeInt(int min, int max)
		{
			std::uniform_int_distribution<int> value(min, max);
			return value(rng);
		}

		float RangeFloat(float min, float max)
		{
			std::uniform_real_distribution<float> value(min, max);
			return value(rng);
		}

		std::mt19937& Engine() { return rng; }

	private:
		std::mt19937 rng;
	};

	// Access to a single engine-wide RNG instance
	//RNG& GlobalRNG();
}

extern CommonUtilities::RNG globalRNG;
