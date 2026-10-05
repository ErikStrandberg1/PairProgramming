#include "DecisionNode.h"

#include <cfloat>

#include "AIEventManager.h"
#include "BanditController.h"
#include "PollingStation.h"

Tga::Vector2f AI::GoToGoalNode::Evaluate(BanditController& aBandit,
                                         [[maybe_unused]] const UpdateContext& aCtx,
                                         [[maybe_unused]] const UpdateMoveContext& aMoveCtx)
{
	return aBandit.GetGoalSpot();
}

Tga::Vector2f AI::HideNode::Evaluate(BanditController& aBandit,
                                     [[maybe_unused]] const UpdateContext& aCtx,
                                     [[maybe_unused]] const UpdateMoveContext& aMoveCtx)
{
	return aBandit.GetHidingTarget();
}

Tga::Vector2f AI::FleeNode::Evaluate([[maybe_unused]] BanditController& aBandit,
                                     [[maybe_unused]] const UpdateContext& aCtx,
                                     [[maybe_unused]] const UpdateMoveContext& aMoveCtx)
{
	auto guardPos = PollingStation::GetInstance().GetGuardPosition();
	auto away = (aMoveCtx.pos - guardPos).GetNormalized();
	return aMoveCtx.pos + away * 0.2f; 
}

Tga::Vector2f AI::HideFromDogNode::Evaluate(BanditController& aBandit,
                                            [[maybe_unused]] const UpdateContext& aCtx,
                                            const UpdateMoveContext& aMoveCtx)
{
	const Tga::Vector2f dogPos = PollingStation::GetInstance().GetDogPosition();

	bool found = false;
	float closestDistSqr = 100.f;
	Tga::Vector2f bestSpot = aMoveCtx.pos;
	for (const auto& spot : aBandit.GetHidingSpots())
	{
		const float distSqr = (spot - aMoveCtx.pos).LengthSqr();
		if (distSqr < closestDistSqr && distSqr < (spot - dogPos).LengthSqr())
		{
			closestDistSqr = distSqr;
			bestSpot = spot;
			found = true;
		}
	}

	if (!found)
	{
		for (const auto& spot : aBandit.GetHidingSpots())
		{
			const float distSqr = (spot - aMoveCtx.pos).LengthSqr();
			if (distSqr < closestDistSqr)
			{
				closestDistSqr = distSqr;
				bestSpot = spot;
			}
		}
	}

	aBandit.SetTargetHidingSpot(bestSpot);
	return bestSpot;
}

AI::ConditionNode::ConditionNode(DecisionNode* aTrueBranch, DecisionNode* aFalseBranch) :
	myTrueBranch(aTrueBranch),
	myFalseBranch(aFalseBranch)
{
}

Tga::Vector2f AI::ConditionNode::Evaluate(BanditController& aBandit,
                                          const UpdateContext& aCtx,
                                          const UpdateMoveContext& aMoveCtx)
{
	if (Condition(aBandit, aCtx, aMoveCtx))
	{
		return myTrueBranch->Evaluate(aBandit, aCtx, aMoveCtx);
	}
	return myFalseBranch->Evaluate(aBandit, aCtx, aMoveCtx);
}

bool AI::IsGuardNearNode::Condition([[maybe_unused]] BanditController& aBandit,
                                    [[maybe_unused]] const UpdateContext& aCtx,
                                    const UpdateMoveContext& aMoveCtx)
{
	auto guardPos = PollingStation::GetInstance().GetGuardPosition();
	constexpr float guardNearRange = 0.18f;

	if ((guardPos - aMoveCtx.pos).LengthSqr() < guardNearRange * guardNearRange)
	{
		return true;
	}
	return false;
}

bool AI::IsDogNearNode::Condition([[maybe_unused]] BanditController& aBandit,
                                  [[maybe_unused]] const UpdateContext& aCtx,
                                  const UpdateMoveContext& aMoveCtx)
{
	constexpr float dogScareRange = 0.15f;
	auto dogPos = PollingStation::GetInstance().GetDogPosition();

	return (dogPos - aMoveCtx.pos).LengthSqr() < dogScareRange * dogScareRange;
}

bool AI::IsHidingSpotNearNode::Condition(BanditController& aBandit,
                                         [[maybe_unused]] const UpdateContext& aCtx,
                                         const UpdateMoveContext& aMoveCtx)
{
	constexpr float searchRange = 0.2f;
	float closestDistSqr = searchRange * searchRange;
	bool found = false;

	for (const auto& spot : aBandit.GetHidingSpots())
	{
		const float distSqr = (spot - aMoveCtx.pos).LengthSqr();
		if (distSqr < closestDistSqr)
		{
			closestDistSqr = distSqr;
			aBandit.SetTargetHidingSpot(spot);
			found = true;
		}
	}

	return found;
}
