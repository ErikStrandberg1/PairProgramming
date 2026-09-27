#include "BanditController.h"

#include "AIEventManager.h"
#include "SteeringBehaviours.h"

AI::BanditController::BanditController()
{
	AIEventManager::GetInstance().Subscribe(this);
}

AI::BanditController::~BanditController()
{
}

Tga::Vector2f AI::BanditController::Update(const UpdateContext& updateContext,
                                           const UpdateMoveContext& aUpdateMoveContext)
{
	if (myHasDied)
	{
		myDeathTimer += updateContext.myDeltaTime;
		if (myDeathTimer >= DEATH_DURATION)
		{
			myHasDied = false;
		}
	}


	auto steering = Steering::Wander(aUpdateMoveContext, myWanderAngle, updateContext.myDeltaTime, myMaxSpeed,
	                                 myMaxForce);
	return steering;
}

void AI::BanditController::OnEvent(const AIEvent& aEvent)
{
	Controller::OnEvent(aEvent);
}

void BanditController::Death()
{
	myHasDied = true;
}
