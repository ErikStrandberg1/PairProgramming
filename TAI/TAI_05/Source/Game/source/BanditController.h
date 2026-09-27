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
		void SetTargetHidingSpot(Tga::Vector2f aHidingSpot) { myTargetSpot = aHidingSpot; }
		Tga::Vector2f GetHidingTarget() const { return myTargetSpot; }

		Tga::Vector2f GetGoalSpot() const { return myGoalSpot; }

	private:
		std::vector<std::unique_ptr<DecisionNode>> myNodes;
		DecisionNode* myRootNode = nullptr;

		std::vector<Tga::Vector2f> myHidingSpots;
		Tga::Vector2f myTargetSpot;

		Tga::Vector2f myGoalSpot;

		Tga::Vector2f myTargetPosition;
		Actor* mySpottedBandit = nullptr;
		float myWanderAngle = 0.f;

		float myDeathTimer = 0.f;
		bool myHasDied = false;

		Tga::Vector2f myStartPosition;
		bool myHasStartPosition = false;
	};
}
