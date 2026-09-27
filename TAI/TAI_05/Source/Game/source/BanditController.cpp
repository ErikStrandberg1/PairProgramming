#include "BanditController.h"

#include "AIEventManager.h"
#include "SteeringBehaviours.h"

AI::BanditController::BanditController()
{
	AIEventManager::GetInstance().Subscribe(this);
	myHidingSpots.emplace_back(0.4f, 0.4f);
	myHidingSpots.emplace_back(0.2f, 0.6f);
	myHidingSpots.emplace_back(0.6f, 0.2f);

	myGoalSpot = {0.1f, 0.5f};

	auto goToGoalNode = std::make_unique<GoToGoalNode>();
	auto fleeNode = std::make_unique<FleeNode>();
	auto hideNode = std::make_unique<HideNode>();

	auto isHidingSpotNear = std::make_unique<IsHidingSpotNearNode>(hideNode.get(), fleeNode.get());
	auto isGuardNear = std::make_unique<IsGuardNearNode>(isHidingSpotNear.get(), goToGoalNode.get());

	myRootNode = isGuardNear.get();

	myNodes.emplace_back(std::move(isGuardNear));
	myNodes.emplace_back(std::move(isHidingSpotNear));
	myNodes.emplace_back(std::move(goToGoalNode));
	myNodes.emplace_back(std::move(hideNode));
	myNodes.emplace_back(std::move(fleeNode));
}

AI::BanditController::~BanditController()
{
}

Tga::Vector2f AI::BanditController::Update(const UpdateContext& updateContext,
                                           const UpdateMoveContext& aUpdateMoveContext)
{
	if (myHasDied)
	{
		myDeathTimer += updateContext.myDeltaTime;
		if (myDeathTimer >= DEATH_DURATION)
		{
			myHasDied = false;
		}
	}

	auto targetPos = myRootNode->Evaluate(*this, updateContext, aUpdateMoveContext);
	auto steer = AI::Steering::Seek(aUpdateMoveContext, targetPos, myMaxSpeed, myMaxForce);

	return steer;
}

void AI::BanditController::OnEvent(const AIEvent& aEvent)
{
	Controller::OnEvent(aEvent);
}

void BanditController::Death()
{
	myHasDied = true;
}
