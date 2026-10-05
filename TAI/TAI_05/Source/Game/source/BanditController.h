#pragma once
#include <vector>

#include "Controller.h"
#include "DecisionNode.h"
#include "State.h"

namespace AI
{
	namespace
	{
		constexpr float DEATH_DURATION = 2.1f;
		constexpr float GOAL_REACHED_RADIUS = 0.01f;
	}
	class BanditController : public Controller
	{
	public:
		BanditController();
		~BanditController();
		virtual Tga::Vector2f Update(const UpdateContext& updateContext,
		                             const UpdateMoveContext& aUpdateMoveContext);

		virtual void OnEvent([[maybe_unused]] const AIEvent& aEvent);
		void Death();
		eControllerType GetType() const override { return eControllerType::eBandit; }

		std::vector<Tga::Vector2f> GetHidingSpots() const { return myHidingSpots; }
		void SetTargetHidingSpot(Tga::Vector2f aHidingSpot);
		Tga::Vector2f GetHidingTarget() const { return myTargetSpot; }

		Tga::Vector2f GetGoalSpot() const { return myGoalSpot; }

		bool HasEscaped() const { return myHasEscaped; }
		int GetCaptures() const { return myCaptures; }

	private:
		std::vector<std::unique_ptr<DecisionNode>> myNodes;
		DecisionNode* myRootNode = nullptr;

		std::vector<Tga::Vector2f> myHidingSpots;
		Tga::Vector2f myTargetSpot;
		Tga::Vector2f myGoalSpot;
		Tga::Vector2f myStartPosition;
		Tga::Vector2f myTargetPosition;

		float myDeathTimer = 0.f;
		bool myHasDied = false;
		bool myHasStartPosition = false;
		bool myHasEscaped = false;
		int myCaptures = 0;
	};
}
