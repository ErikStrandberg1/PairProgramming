#include "DecisionNode.h"

#include "BanditController.h"
#include "PollingStation.h"

Tga::Vector2f AI::GoToGoalNode::Evaluate(BanditController& aBandit, [[maybe_unused]] const UpdateContext& aCtx,
                                         [[maybe_unused]] const UpdateMoveContext& aMoveCtx)
{
	return aBandit.GetGoalSpot();
}

Tga::Vector2f AI::HideNode::Evaluate(BanditController& aBandit, [[maybe_unused]] const UpdateContext& aCtx,
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

AI::ConditionNode::ConditionNode(DecisionNode* aTrueBranch, DecisionNode* aFalseBranch) :
	myTrueBranch(aTrueBranch),
	myFalseBranch(aFalseBranch)
{
}

Tga::Vector2f AI::ConditionNode::Evaluate(BanditController& aBandit, const UpdateContext& aCtx,
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

	if ((guardPos - aMoveCtx.pos).LengthSqr() < 0.2f * 0.2f)
	{
		return true;
	}
	return false;
}

bool AI::IsHidingSpotNearNode::Condition(BanditController& aBandit, [[maybe_unused]] const UpdateContext& aCtx,
                                         const UpdateMoveContext& aMoveCtx)
{
	auto hidingSpots = aBandit.GetHidingSpots();
	for (const auto& spot : hidingSpots)
	{
		if ((spot - aMoveCtx.pos).LengthSqr() < 0.2f * 0.2f)
		{
			aBandit.SetTargetHidingSpot(spot);
			return true;
		}
	}

	return false;
}
