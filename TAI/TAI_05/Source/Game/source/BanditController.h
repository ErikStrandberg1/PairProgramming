#pragma once
#include <vector>

#include "Controller.h"
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

	private:
		std::vector<std::unique_ptr<State>> myAvailableStates;
		State* myState = nullptr;

		Tga::Vector2f myTargetPosition;
		Actor* mySpottedBandit = nullptr;
		float myWanderAngle = 0.f;

		float myDeathTimer = 0.f;
		bool myHasDied = false;

		Tga::Vector2f myStartPosition;
		bool myHasStartPosition = false;
	};
}
