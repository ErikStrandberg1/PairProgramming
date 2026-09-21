#include "SeparationController.h"
#include "GameWorld.h"
#include "UpdateContext.h"

#include "Controller.h"
#include "SteeringBehaviours.h"

SeparationController::SeparationController(eSteeringType aSteeringType)
{
	AIEventManager::GetInstance().Subscribe(this);
	mySteeringType = aSteeringType;
}

Tga::Vector2f SeparationController::Update([[maybe_unused]] const UpdateContext& updateContext,
                                           const UpdateMoveContext& aUpdateMoveContext)
{
	auto steering = Steering::Separation(aUpdateMoveContext, 10.f, myMaxForce);
	return steering;
}

