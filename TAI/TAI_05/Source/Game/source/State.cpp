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
}

Tga::Vector2f AI::IdleState::Update([[maybe_unused]] AI::GuardController& aGuardController,
                                    [[maybe_unused]] const UpdateContext& aCtx,
                                    [[maybe_unused]] const UpdateMoveContext& aMoveCtx)
{
	if (aGuardController.GetStateTimer() <= REST_DURATION)
	{
		return aMoveCtx.pos; // stay in place
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
		std::cout << "Guard has captured the bandit!\n";
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

Tga::Vector2f AI::PatrolState::Update([[maybe_unused]] AI::GuardController& aGuardController,
                                      [[maybe_unused]] const UpdateContext& aCtx,
                                      [[maybe_unused]] const UpdateMoveContext& aMoveCtx)
{
	if (aGuardController.GetStateTimer() >= PATROL_DURATION)
	{
		aGuardController.SetState(GuardStates::Idle);
		return Tga::Vector2f{};
	}

	if (aGuardController.CanSeeBandit(aMoveCtx.pos))
	{
		AIEvent event;
		event.myType = AIEvent::Type::GuardSpottedBandit;
		AIEventManager::GetInstance().SendEvent(event);
		std::cout << "Guard has spotted the bandit!\n";
		aGuardController.SetState(GuardStates::Chase);

		return Tga::Vector2f{};
	}

	auto waypoints = aGuardController.GetWayPoints();
	auto currentIdx = aGuardController.GetCurrentWayPoint();
	if ((waypoints[currentIdx] - aMoveCtx.pos).LengthSqr() < ARRIVE_RADIUS * ARRIVE_RADIUS)
	{
		aGuardController.NextWayPoint();
	}
	return waypoints[currentIdx];
}
