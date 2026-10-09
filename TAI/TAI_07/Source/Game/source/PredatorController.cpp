#include "PredatorController.h"

#include "Actor.h"
#include "GameWorld.h"
#include "PollingStation.h"
#include "SteeringBehaviours.h"

namespace AI
{
	constexpr float PREDATOR_WANDER_SPEED = 0.04f;
	constexpr float PREDATOR_CHASE_DISTANCE = 0.3f;
	constexpr float PREDATOR_EAT_DISTANCE = 0.015f;
	constexpr float PREDATOR_MIN_LIFETIME = 30.f;
	constexpr float PREDATOR_MAX_LIFETIME = 50.f;
	constexpr float PREDATOR_FENCE_MARGIN = 0.04f;
}

AI::PredatorController::PredatorController()
{
	myMaxForce = 0.5f;
	myLifeTime = RandomRange(PREDATOR_MIN_LIFETIME, PREDATOR_MAX_LIFETIME);
}

Tga::Vector2f AI::PredatorController::Update(const UpdateContext& updateContext,
                                             const UpdateMoveContext& aUpdateMoveContext)
{
	myAge += updateContext.myDeltaTime;

	if (myAge >= myLifeTime && PollingStation::GetInstance().GetAlivePredatorCount() > 1)
	{
		myOwner->Kill();
		return Tga::Vector2f{};
	}

	Tga::Vector2f steering = {};
	switch (myState)
	{
		case PredatorStates::Wander:
		{
			steering = UpdateWander(updateContext, aUpdateMoveContext);
			break;
		}
		case PredatorStates::Hunt:
		{
			steering = UpdateHunt(updateContext, aUpdateMoveContext);
			break;
		}
	}

	steering += Steering::StayInside(aUpdateMoveContext,
	                                 updateContext.myGameWold->GetFenceMin(),
	                                 updateContext.myGameWold->GetFenceMax(),
	                                 PREDATOR_FENCE_MARGIN,
	                                 myMaxForce);
	return steering;
}

void AI::PredatorController::SetState(PredatorStates aState)
{
	myState = aState;
}

Tga::Vector2f AI::PredatorController::UpdateWander(const UpdateContext& aCtx, const UpdateMoveContext& aMoveCtx)
{
	if (PollingStation::GetInstance().GetClosestPrey(aMoveCtx.pos, PREDATOR_CHASE_DISTANCE) != nullptr)
	{
		SetState(PredatorStates::Hunt);
		return UpdateHunt(aCtx, aMoveCtx);
	}

	return Steering::Wander(aMoveCtx, myWanderAngle, aCtx.myDeltaTime, PREDATOR_WANDER_SPEED, myMaxForce);
}

Tga::Vector2f AI::PredatorController::UpdateHunt([[maybe_unused]] const UpdateContext& aCtx,
                                                 const UpdateMoveContext& aMoveCtx)
{
	Actor* prey = PollingStation::GetInstance().GetClosestPrey(aMoveCtx.pos, PREDATOR_CHASE_DISTANCE);
	if (prey == nullptr)
	{
		SetState(PredatorStates::Wander);
		return Tga::Vector2f{};
	}

	if (IsWithinRange(prey->GetPosition(), aMoveCtx.pos, PREDATOR_EAT_DISTANCE))
	{
		prey->Kill();
	}
	auto steer = Steering::Seek(aMoveCtx, prey->GetPosition(), myMaxSpeed, myMaxForce);
	return steer;
}
