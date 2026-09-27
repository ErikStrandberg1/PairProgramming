#pragma once
#include "UpdateContext.h"

namespace AI
{
	class BanditController;

	class DecisionNode
	{
	public:
		virtual ~DecisionNode() = default;
		virtual Tga::Vector2f Evaluate(BanditController& aBandit, const UpdateContext& aCtx,
		                               const UpdateMoveContext& aMoveCtx) = 0;
	};

	class GoToGoalNode : public DecisionNode
	{
	public:
		Tga::Vector2f Evaluate(BanditController& aBandit, const UpdateContext& aCtx,
		                       const UpdateMoveContext& aMoveCtx) override;
	};

	class HideNode : public DecisionNode
	{
	public:
		Tga::Vector2f Evaluate(BanditController& aBandit, const UpdateContext& aCtx,
		                       const UpdateMoveContext& aMoveCtx) override;
	};

	class FleeNode : public DecisionNode
	{
	public:
		Tga::Vector2f Evaluate(BanditController& aBandit, const UpdateContext& aCtx,
		                       const UpdateMoveContext& aMoveCtx) override;
	};

	class ConditionNode : public DecisionNode
	{
	public:
		ConditionNode(DecisionNode* aTrueBranch, DecisionNode* aFalseBranch);
		Tga::Vector2f Evaluate(BanditController& aBandit, const UpdateContext& aCtx,
		                       const UpdateMoveContext& aMoveCtx) override;

	protected:
		virtual bool Condition(BanditController& aBandit, const UpdateContext& aCtx,
		                       const UpdateMoveContext& aMoveCtx) = 0;

	private:
		DecisionNode* myTrueBranch;
		DecisionNode* myFalseBranch;
	};

	class IsGuardNearNode : public ConditionNode
	{
	public:
		using ConditionNode::ConditionNode;

	protected:
		bool Condition(BanditController& aBandit, const UpdateContext& aCtx,
		               const UpdateMoveContext& aMoveCtx) override;
	};

	class IsHidingSpotNearNode : public ConditionNode
	{
	public:
		using ConditionNode::ConditionNode;

	protected:
		bool Condition(BanditController& aBandit, const UpdateContext& aCtx,
		               const UpdateMoveContext& aMoveCtx) override;
	};
}
