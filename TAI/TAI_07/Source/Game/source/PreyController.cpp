#include "PreyController.h"

#include "Actor.h"
#include "GameWorld.h"
#include "PollingStation.h"
#include "SteeringBehaviours.h"

namespace AI
{
	constexpr float PREY_WANDER_SPEED = 0.03f;
	constexpr float PREY_FLEE_DISTANCE = 0.15f;
	constexpr float PREY_SAFE_DISTANCE = 0.22f;
	constexpr float PREY_FENCE_MARGIN = 0.04f;
}

AI::PreyController::PreyController()
{
	myMaxForce = 0.4f;
}

Tga::Vector2f AI::PreyController::Update(const UpdateContext& updateContext,
                                         const UpdateMoveContext& aUpdateMoveContext)
{
	Tga::Vector2f steering = {};
	switch (myState)
	{
		case PreyStates::Wander:
		{
			steering = UpdateWander(updateContext, aUpdateMoveContext);
			break;
		}
		case PreyStates::Flee:
		{
			steering = UpdateFlee(aUpdateMoveContext);
			break;
		}
	}

	steering += Steering::StayInside(aUpdateMoveContext,
	                                 updateContext.myGameWold->GetFenceMin(),
	                                 updateContext.myGameWold->GetFenceMax(),
	                                 PREY_FENCE_MARGIN,
	                                 myMaxForce);
	return steering;
}

Tga::Vector2f AI::PreyController::UpdateWander(const UpdateContext& aCtx, const UpdateMoveContext& aMoveCtx)
{
	if (PollingStation::GetInstance().GetClosestPredator(aMoveCtx.pos, PREY_FLEE_DISTANCE) != nullptr)
	{
		myState = PreyStates::Flee;
		return Tga::Vector2f{};
	}

	return Steering::Wander(aMoveCtx, myWanderAngle, aCtx.myDeltaTime, PREY_WANDER_SPEED, myMaxForce);
}

Tga::Vector2f AI::PreyController::UpdateFlee(const UpdateMoveContext& aMoveCtx)
{
	Actor* predator = PollingStation::GetInstance().GetClosestPredator(aMoveCtx.pos, PREY_SAFE_DISTANCE);
	if (predator == nullptr)
	{
		myState = PreyStates::Wander;
		return Tga::Vector2f{};
	}

	const Tga::Vector2f away = (aMoveCtx.pos - predator->GetPosition()).GetNormalized();
	return Steering::Seek(aMoveCtx, aMoveCtx.pos + away * 0.2f, myMaxSpeed, myMaxForce);
}
