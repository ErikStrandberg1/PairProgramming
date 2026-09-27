#include "BanditController.h"
#include "Actor.h"
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

	if (!myHasStartPosition)
	{
		myStartPosition = aUpdateMoveContext.pos;
		myHasStartPosition = true;
	}

	if (myHasDied)
	{
		myDeathTimer += updateContext.myDeltaTime;
		if (myDeathTimer >= DEATH_DURATION)
		{
			myHasDied = false;
			myDeathTimer = 0.f;
			if (myOwner != nullptr)
			{
				myOwner->Teleport(myStartPosition);
			}
		}
		return Tga::Vector2f{}; 
	}

	auto steering = Steering::Wander(aUpdateMoveContext, myWanderAngle, updateContext.myDeltaTime, myMaxSpeed,
	                                 myMaxForce);
	return steering;
}

void AI::BanditController::OnEvent(const AIEvent& aEvent)
{
	if (aEvent.myType == AIEvent::Type::BanditCaptured)
	{
		Death();
		return;
	}
	Controller::OnEvent(aEvent);
}

void BanditController::Death()
{
	myHasDied = true;
	myDeathTimer = 0.f;
}
