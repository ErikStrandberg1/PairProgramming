#include "WorldDirector.h"

#include "Actor.h"
#include "Controller.h"
#include "GameWorld.h"
#include "PollingStation.h"

namespace AI
{
	constexpr float INCREASE_PREY_REPRODUCTION_INTERVAL = 0.6f;
	constexpr float BALANCE_REPRODUCTION_INTERVAL = 1.85f;
	constexpr float DECREASE_PREY_REPRODUCTION_INTERVAL = 4.f;
	constexpr float PREY_SPAWN_OFFSET = 0.03f;
	constexpr int MAX_PREY = 60;
}

void AI::WorldDirector::Update(GameWorld& aGameWorld, float aDeltaTime)
{
	const int preyCount = static_cast<int>(PollingStation::GetInstance().GetPreys().size());

	if (preyCount <= myIncreasePreyThreshold)
	{
		SetState(DirectorStates::IncreasePrey);
	}
	else if (preyCount >= myDecreasePreyThreshold)
	{
		SetState(DirectorStates::DecreasePrey);
	}
	else
	{
		SetState(DirectorStates::Balance);
	}

	switch (myState)
	{
		case DirectorStates::IncreasePrey:
		{
			myPreyReproductionInterval = INCREASE_PREY_REPRODUCTION_INTERVAL;
			break;
		}
		case DirectorStates::Balance:
		{
			myPreyReproductionInterval = BALANCE_REPRODUCTION_INTERVAL;
			break;
		}
		case DirectorStates::DecreasePrey:
		{
			myPreyReproductionInterval = DECREASE_PREY_REPRODUCTION_INTERVAL;
			break;
		}
	}

	UpdatePreyReproduction(aGameWorld, aDeltaTime);
}

const char* AI::WorldDirector::GetStateName() const
{
	switch (myState)
	{
		case DirectorStates::IncreasePrey:
		{
			return "IncreasePrey";
		}
		case DirectorStates::Balance:
		{
			return "Balance";
		}
		case DirectorStates::DecreasePrey:
		{
			return "DecreasePrey";
		}
	}
	return "";
}

void AI::WorldDirector::SetState(DirectorStates aState)
{
	myState = aState;
}

void AI::WorldDirector::UpdatePreyReproduction(GameWorld& aGameWorld, float aDeltaTime)
{
	myPreyReproductionTimer += aDeltaTime;
	if (myPreyReproductionTimer < myPreyReproductionInterval)
	{
		return;
	}
	myPreyReproductionTimer = 0.f;

	const std::vector<Actor*>& preys = PollingStation::GetInstance().GetPreys();
	if (static_cast<int>(preys.size()) >= MAX_PREY)
	{
		return;
	}

	if (preys.empty())
	{
		aGameWorld.SpawnPrey(aGameWorld.GetRandomPositionInsideFence());
		return;
	}

	const Actor* parent = preys[rand() % preys.size()];
	const Tga::Vector2f offset = {RandomRange(-PREY_SPAWN_OFFSET, PREY_SPAWN_OFFSET),
	                              RandomRange(-PREY_SPAWN_OFFSET, PREY_SPAWN_OFFSET)};
	aGameWorld.SpawnPrey(parent->GetPosition() + offset);
}
