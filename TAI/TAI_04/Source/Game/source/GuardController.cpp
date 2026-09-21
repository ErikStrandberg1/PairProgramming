#include "GuardController.h"
#include "AIEventManager.h"
#include "SteeringBehaviours.h"

using namespace AI;

AI::GuardController::GuardController()
{
	AIEventManager::GetInstance().Subscribe(this);

	myAvailableStates.emplace_back(std::make_unique<IdleState>());
	myAvailableStates.emplace_back(std::make_unique<ChaseState>());
	myAvailableStates.emplace_back(std::make_unique<PatrolState>());

	myState = myAvailableStates.back().get();
}

AI::GuardController::~GuardController()
{
}

Tga::Vector2f GuardController::Update(const UpdateContext& updateContext, const UpdateMoveContext& aUpdateMoveContext)
{
	if (myState)
	{
		auto targetPos = myState->Update(*this, updateContext, aUpdateMoveContext);
		auto steer = AI::Steering::Seek(aUpdateMoveContext, targetPos, myMaxSpeed, myMaxForce);

		return steer;
	}

	return Controller::Update(updateContext, aUpdateMoveContext);
}

void GuardController::OnEvent(const AIEvent& aEvent)
{
	Controller::OnEvent(aEvent);
}
