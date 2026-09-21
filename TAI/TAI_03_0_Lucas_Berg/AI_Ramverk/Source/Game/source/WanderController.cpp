#include "stdafx.h"
#include "GameWorld.h"
#include "UpdateContext.h"

#include "Controller.h"
#include "SteeringBehaviours.h"
#include "WanderController.h"

WanderController::WanderController(eSteeringType aSteeringType)
{
	AIEventManager::GetInstance().Subscribe(this);
	mySteeringType = aSteeringType;
}

Tga::Vector2f WanderController::Update(const UpdateContext& updateContext, const UpdateMoveContext& aUpdateMoveContext)
{
	auto steering = Steering::Wander(aUpdateMoveContext, myWanderAngle, updateContext.myDeltaTime, myMaxSpeed,
	                                 myMaxForce);
	return steering;
}
