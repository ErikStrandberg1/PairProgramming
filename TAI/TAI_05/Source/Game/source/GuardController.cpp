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

	myPointOfInterests.emplace_back(0.1f, 0.2f);
	myPointOfInterests.emplace_back(0.1f, 0.2f);
	myPointOfInterests.emplace_back(0.3f, 0.72f);
	myPointOfInterests.emplace_back(0.7f, 0.52f);
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

	auto banditPos = PollingStation::GetInstance().GetBanditPosition();
	constexpr float catchRange = 0.01f;
	if ((aUpdateMoveContext.pos - banditPos).LengthSqr() > catchRange * catchRange)
	{
		AIEvent event;
		event.myType = AIEvent::Type::GuardSpottedBandit;
		AIEventManager::GetInstance().SendEvent(event);
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
	auto banditPos = PollingStation::GetInstance().GetBanditPosition();
	if ((banditPos - aMyPosition).LengthSqr() < VISION_RANGE * VISION_RANGE)
	{
		return true;
	}
	return false;
}
