#include "GuardController.h"
#include "AIEventManager.h"
#include "PollingStation.h"
#include "SteeringBehaviours.h"
#include "UpdateContext.h"
#include "Controller.h"
#include "State.h"

using namespace AI;

AI::GuardController::GuardController()
{
	AIEventManager::GetInstance().Subscribe(this);
	myAvailableStates.emplace_back(std::make_unique<IdleState>());
	myAvailableStates.emplace_back(std::make_unique<PatrolState>());
	myAvailableStates.emplace_back(std::make_unique<ChaseState>());
	SetState(GuardStates::Patrol);

	myPointOfInterests.emplace_back(0.15f, 0.5f);
	myPointOfInterests.emplace_back(0.25f, 0.25f);
	myPointOfInterests.emplace_back(0.25f, 0.75f);
	myPointOfInterests.emplace_back(0.45f, 0.35f);
	myPointOfInterests.emplace_back(0.45f, 0.7f);
	myPointOfInterests.emplace_back(0.65f, 0.5f);
	myPointOfInterests.emplace_back(0.75f, 0.25f);
	myPointOfInterests.emplace_back(0.75f, 0.75f);
	myInterestIdx = rand() % static_cast<int>(myPointOfInterests.size());
}

void GuardController::NextWayPoint()
{
	const int count = static_cast<int>(myPointOfInterests.size());
	int next = rand() % count;
	if (next == myInterestIdx)
	{
		next = (next + 1) % count;
	}
	myInterestIdx = next;
}

AI::GuardController::~GuardController()
{
}

Tga::Vector2f GuardController::Update(const UpdateContext& updateContext, const UpdateMoveContext& aUpdateMoveContext)
{
	if (myState == nullptr)
	{
		return Tga::Vector2f{};
	}

	myStateTimer += updateContext.myDeltaTime;

	auto targetPos = myState->Update(*this, updateContext, aUpdateMoveContext);
	auto steer = AI::Steering::Seek(aUpdateMoveContext, targetPos, myMaxSpeed, myMaxForce);

	return steer;
}

void GuardController::OnEvent(const AIEvent& aEvent)
{
	Controller::OnEvent(aEvent);
}

void GuardController::SetState(GuardStates aGuardState)
{
	myState = myAvailableStates[(int)aGuardState].get();
	myStateTimer = 0.f;
}

bool GuardController::CanSeeBandit(const Tga::Vector2f aMyPosition) const
{
	const Tga::Vector2f banditPos = PollingStation::GetInstance().GetBanditPosition();
	auto hidingSpots = PollingStation::GetInstance().GetHidingSpots();

	for (auto hidingSpot : hidingSpots)
	{
		if ((banditPos - hidingSpot).LengthSqr() < HIDE_RADIUS * HIDE_RADIUS)
		{
			return false;
		}
	}
	if ((banditPos - aMyPosition).LengthSqr() < VISION_RANGE * VISION_RANGE)
	{
		return true;
	}
	return false;
}

bool GuardController::IsBanditCaptured(const Tga::Vector2f aMyPosition) const
{
	auto banditPos = PollingStation::GetInstance().GetBanditPosition();
	if ((banditPos - aMyPosition).LengthSqr() < CAPTURE_RANGE * CAPTURE_RANGE)
	{
		return true;
	}
	return false;
}
