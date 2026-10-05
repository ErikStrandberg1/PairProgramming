#include "State.h"

#include "Actor.h"
#include "GuardController.h"
#include "PollingStation.h"
#include "AIEventManager.h"

namespace AI
{
	constexpr float REST_DURATION = 2.f;
	constexpr float PATROL_DURATION = 6.f;
	constexpr float ARRIVE_RADIUS = 0.03f;
	constexpr float GO_TO_DOG_DURATION = 4.f;
	constexpr float LOOK_ARRIVE_RADIUS = 0.1f;
}

Tga::Vector2f AI::IdleState::Update([[maybe_unused]] AI::GuardController& aGuardController,
                                    [[maybe_unused]] const UpdateContext& aCtx,
                                    [[maybe_unused]] const UpdateMoveContext& aMoveCtx)
{
	if (aGuardController.GetStateTimer() <= REST_DURATION)
	{
		return aMoveCtx.pos; 
	}
	else
	{
		aGuardController.SetState(GuardStates::Patrol);
		return Tga::Vector2f{};
	}
}

Tga::Vector2f AI::ChaseState::Update([[maybe_unused]] AI::GuardController& aGuardController,
                                     [[maybe_unused]] const UpdateContext& aCtx,
                                     [[maybe_unused]] const UpdateMoveContext& aMoveCtx)
{
	if (aGuardController.IsBanditCaptured(aMoveCtx.pos))
	{
		AIEvent event;
		event.myType = AIEvent::Type::BanditCaptured;
		AIEventManager::GetInstance().SendEvent(event);
		aGuardController.SetState(GuardStates::Idle);
		return Tga::Vector2f{};
	}

	if (aGuardController.CanSeeBandit(aMoveCtx.pos))
	{
		auto banditPos = PollingStation::GetInstance().GetBanditPosition();
		return banditPos;
	}
	aGuardController.SetState(GuardStates::Idle);
	return Tga::Vector2f{};
}

Tga::Vector2f AI::PatrolState::Update(AI::GuardController& aGuardController,
                                      [[maybe_unused]] const UpdateContext& aCtx,
                                      const UpdateMoveContext& aMoveCtx)
{
	if (aGuardController.HasDogFoundBandit())
	{
		aGuardController.ClearDogFoundBandit();
		aGuardController.SetState(GuardStates::GoToDog);
		return aMoveCtx.pos;
	}
	if (aGuardController.GetStateTimer() >= PATROL_DURATION)
	{
		aGuardController.SetState(GuardStates::Idle);
		return aMoveCtx.pos;
	}
	if (aGuardController.TryStartChase(aMoveCtx.pos))
	{
		return aMoveCtx.pos;
	}

	const auto& waypoints = aGuardController.GetWayPoints();
	const int currentIdx = aGuardController.GetCurrentWayPoint();
	if (IsWithinRange(waypoints[currentIdx], aMoveCtx.pos, ARRIVE_RADIUS))
	{
		aGuardController.NextWayPoint();
	}
	return waypoints[currentIdx];
}

Tga::Vector2f AI::GoToDogState::Update(AI::GuardController& aGuardController,
                                       [[maybe_unused]] const UpdateContext& aCtx,
                                       const UpdateMoveContext& aMoveCtx)
{
	if (aGuardController.TryStartChase(aMoveCtx.pos))
	{
		return aMoveCtx.pos;
	}

	const Tga::Vector2f& target = aGuardController.GetDogFoundBanditPos();
	const bool arrived = IsWithinRange(target, aMoveCtx.pos, LOOK_ARRIVE_RADIUS);
	if (arrived || aGuardController.GetStateTimer() >= GO_TO_DOG_DURATION)
	{
		aGuardController.ClearDogFoundBandit();
		aGuardController.SetState(GuardStates::Idle);
		return aMoveCtx.pos;
	}
	return target;
}
