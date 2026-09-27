#include "DecisionNode.h"

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
