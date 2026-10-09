#pragma once

class GameWorld;

namespace AI
{
	enum class DirectorStates
	{
		IncreasePrey,
		Balance,
		DecreasePrey,
	};

	class WorldDirector
	{
	public:
		WorldDirector() = default;

		void Update(GameWorld& aGameWorld, float aDeltaTime);

		DirectorStates GetState() const { return myState; }
		const char* GetStateName() const;

		int GetIncreasePreyThreshold() const { return myIncreasePreyThreshold; }
		int GetDecreasePreyThreshold() const { return myDecreasePreyThreshold; }
		float GetPreyReproductionInterval() const { return myPreyReproductionInterval; }

	private:
		void SetState(DirectorStates aState);
		void UpdatePreyReproduction(GameWorld& aGameWorld, float aDeltaTime);

		DirectorStates myState = DirectorStates::Balance;

		int myIncreasePreyThreshold = 10;
		int myDecreasePreyThreshold = 40;

		float myPreyReproductionInterval = 1.85f;
		float myPreyReproductionTimer = 0.f;
	};
}
