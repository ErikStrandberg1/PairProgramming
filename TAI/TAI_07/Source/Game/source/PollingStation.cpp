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
