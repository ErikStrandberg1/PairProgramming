#include "PollingStation.h"

#include "Controller.h"

AI::PollingStation& AI::PollingStation::GetInstance()
{
    static PollingStation instance;
    return instance;
}

void AI::PollingStation::Init(std::vector<Actor*> aActors, Actor* aPlayer, std::vector<Tga::Vector2f> aHidingSpots)
{
	myActors = std::move(aActors);
	myPlayer = aPlayer;
	myHidingSpots = aHidingSpots;

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

std::vector<Tga::Vector2f> AI::PollingStation::GetHidingSpots() const
{
	return myHidingSpots;
}
