#include "PollingStation.h"

#include "Controller.h"

AI::PollingStation& AI::PollingStation::GetInstance()
{
    static PollingStation instance;
    return instance;
}

void AI::PollingStation::Init(std::vector<Actor*> aActors, Actor* aPlayer, std::vector<Tga::Vector2f> aHidingSpots,
                              std::vector<Tga::Vector2f> aWaterSpots)
{
	myActors = std::move(aActors);
	myPlayer = aPlayer;
	myHidingSpots = aHidingSpots;
	myWaterSpots = aWaterSpots;

	for (auto& actor : myActors)
	{
		if (!actor || !actor->GetController()) { continue; }
		if (actor->GetController()->GetType() == eControllerType::eBandit)
		{
			myBandit = actor;
		}
		if (actor->GetController()->GetType() == eControllerType::eGuard)
		{
			myGuard = actor;
		}
		if (actor->GetController()->GetType() == eControllerType::eDog)
		{
			myDog = actor;
		}
	}
}

Tga::Vector2f AI::PollingStation::GetPlayerPosition() const
{
	if (myPlayer != nullptr)
    {
	    return myPlayer->GetPosition();
    }

    return Tga::Vector2f();
}

Tga::Vector2f AI::PollingStation::GetGuardPosition() const
{
	if (myGuard != nullptr)
	{
		return myGuard->GetPosition();
	}

	return Tga::Vector2f();
}

Tga::Vector2f AI::PollingStation::GetBanditPosition() const
{
	if (myBandit != nullptr)
	{
		return myBandit->GetPosition();
	}

	return Tga::Vector2f();
}

Tga::Vector2f AI::PollingStation::GetDogPosition() const
{
	if (myDog != nullptr)
	{
		return myDog->GetPosition();
	}

	return Tga::Vector2f();
}

std::vector<Tga::Vector2f> AI::PollingStation::GetHidingSpots() const
{
	return myHidingSpots;
}

std::vector<Tga::Vector2f> AI::PollingStation::GetWaterSpots() const
{
	return myWaterSpots;
}

bool AI::PollingStation::IsBanditHiding(Tga::Vector2f* outHidingSpot) const
{
	const Tga::Vector2f banditPos = GetBanditPosition();
	for (const auto& spot : myHidingSpots)
	{
		if (IsWithinRange(banditPos, spot, HIDE_RADIUS))
		{
			if (outHidingSpot != nullptr)
			{
				*outHidingSpot = spot;
			}
			return true;
		}
	}
	return false;
}

void AI::PollingStation::SetPreys(std::vector<Actor*> aPreys)
{
	myPreys = std::move(aPreys);
}

void AI::PollingStation::SetPredators(std::vector<Actor*> aPredators)
{
	myPredators = std::move(aPredators);
}

Actor* AI::PollingStation::GetClosestPrey(const Tga::Vector2f& aPosition, float aRange) const
{
	return GetClosestActor(myPreys, aPosition, aRange);
}

Actor* AI::PollingStation::GetClosestPredator(const Tga::Vector2f& aPosition, float aRange) const
{
	return GetClosestActor(myPredators, aPosition, aRange);
}

Actor* AI::PollingStation::GetClosestActor(const std::vector<Actor*>& aActors, const Tga::Vector2f& aPosition,
                                           float aRange) const
{
	Actor* closest = nullptr;
	float closestDistSqr = aRange * aRange;
	for (Actor* actor : aActors)
	{
		if (actor->IsDead())
		{
			continue;
		}

		const float distSqr = (actor->GetPosition() - aPosition).LengthSqr();
		if (distSqr < closestDistSqr)
		{
			closestDistSqr = distSqr;
			closest = actor;
		}
	}
	return closest;
}

int AI::PollingStation::GetAlivePredatorCount() const
{
	int count = 0;
	for (Actor* predator : myPredators)
	{
		if (!predator->IsDead())
		{
			++count;
		}
	}
	return count;
}
