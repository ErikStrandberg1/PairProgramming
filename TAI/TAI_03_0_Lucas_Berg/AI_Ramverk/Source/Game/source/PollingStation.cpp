#include "PollingStation.h"

#include "Controller.h"

AI::PollingStation& AI::PollingStation::GetInstance()
{
    static PollingStation instance;
    return instance;
}

void AI::PollingStation::Init(std::vector<Actor*> aActors, Actor* aPlayer)
{
	myActors = std::move(aActors);
	myPlayer = aPlayer;
}

Tga::Vector2f AI::PollingStation::GetPlayerPosition() const
{
	if (myPlayer != nullptr)
    {
	    return myPlayer->GetPosition();
    }

    return Tga::Vector2f();
}
