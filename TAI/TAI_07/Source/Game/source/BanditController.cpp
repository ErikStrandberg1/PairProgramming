#include "BanditController.h"
#include "Actor.h"
#include "AIEventManager.h"
#include "PollingStation.h"
#include "SteeringBehaviours.h"

AI::BanditController::BanditController()
{
	AIEventManager::GetInstance().Subscribe(this);

	myGoalSpot = {0.05f, RandomRange(0.25f, 0.75f)};

	auto goToGoalNode = std::make_unique<GoToGoalNode>();
	auto fleeNode = std::make_unique<FleeNode>();
	auto hideNode = std::make_unique<HideNode>();
	auto hideFromDogNode = std::make_unique<HideFromDogNode>();

	auto isHidingSpotNear = std::make_unique<IsHidingSpotNearNode>(hideNode.get(), fleeNode.get());
	auto isDogNear = std::make_unique<IsDogNearNode>(hideFromDogNode.get(), goToGoalNode.get());
	auto isGuardNear = std::make_unique<IsGuardNearNode>(isHidingSpotNear.get(), isDogNear.get());

	myRootNode = isGuardNear.get();

	myNodes.emplace_back(std::move(isDogNear));
	myNodes.emplace_back(std::move(hideFromDogNode));
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
	myHidingSpots = PollingStation::GetInstance().GetHidingSpots();
	if (!myHasStartPosition)
	{
		myStartPosition = aUpdateMoveContext.pos;
		myHasStartPosition = true;
	}
	if (IsWithinRange(aUpdateMoveContext.pos, myGoalSpot, GOAL_REACHED_RADIUS))
	{
		AIEvent event;
		event.myType = AIEvent::Type::BanditReachedGoal;
		AIEventManager::GetInstance().SendEvent(event);
	}
	if (myHasDied)
	{
		myDeathTimer += updateContext.myDeltaTime;
		if (myDeathTimer >= DEATH_DURATION)
		{
			myHasDied = false;
			myDeathTimer = 0.f;
			if (myOwner != nullptr)
			{
				myOwner->Teleport({myStartPosition.x, RandomRange(0.2f, 0.8f)});
			}
			myGoalSpot = {0.05f, RandomRange(0.25f, 0.75f)};
		}
		return Tga::Vector2f{}; 
	}
	auto target = myRootNode->Evaluate(*this, updateContext, aUpdateMoveContext);
	auto steering = Steering::Seek(aUpdateMoveContext, target, myMaxSpeed, myMaxForce);
	return steering;
}

void AI::BanditController::OnEvent(const AIEvent& aEvent)
{
	if (aEvent.myType == AIEvent::Type::BanditCaptured)
	{
		++myCaptures;
		Death();
		return;
	}
	if (aEvent.myType == AIEvent::Type::BanditReachedGoal)
	{
		myHasEscaped = true;
		return;
	}

	Controller::OnEvent(aEvent);
}

void BanditController::Death()
{
	myHasDied = true;
	myDeathTimer = 0.f;
}

void BanditController::SetTargetHidingSpot(Tga::Vector2f aHidingSpot)
{
	myTargetSpot = aHidingSpot;
	AIEvent event;
	event.myType = AIEvent::Type::BanditHiding;
	AIEventManager::GetInstance().SendEvent(event);
}

