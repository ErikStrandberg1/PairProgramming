#include "SeekController.h"
#include "AIEventManager.h"
#include "PollingStation.h"
#include "SteeringBehaviours.h"

SeekController::SeekController(eSteeringType aSteeringType)
{
	AIEventManager::GetInstance().Subscribe(this);
	mySteeringType = aSteeringType;
}

Tga::Vector2f SeekController::Update([[maybe_unused]] const UpdateContext& updateContext,
                                     const UpdateMoveContext& aUpdateMoveContext)
{
	myTargetPosition = PollingStation::GetInstance().GetPlayerPosition();
	auto steering = Steering::Seek(aUpdateMoveContext, myTargetPosition, myMaxSpeed, myMaxForce);
	return steering;
}
