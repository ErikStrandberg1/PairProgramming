#pragma once
#include <span>
#include <vector>

#include "Controller.h"
#include "State.h"

struct UpdateContext;

namespace AI
{
	namespace
	{
		constexpr float VISION_RANGE = 0.1f;
	}

	class GuardController : public Controller
	{
	public:
		GuardController();
		~GuardController();
		virtual Tga::Vector2f Update(const UpdateContext& updateContext,
		                             const UpdateMoveContext& aUpdateMoveContext);

		virtual void OnEvent([[maybe_unused]] const AIEvent& aEvent);
		eControllerType GetType() const override { return eControllerType::eGuard; }
		void SetState(GuardStates aGuardState);
		float GetStateTimer() const { return myStateTimer; }

		bool CanSeeBandit(const Tga::Vector2f aMyPosition) const;

		int GetCurrentWayPoint() const { return myInterestIdx; }
		std::vector<Tga::Vector2f> GetWayPoints() const { return myPointOfInterests; }
		void NextWayPoint() { myInterestIdx = (myInterestIdx + 1) % static_cast<int>(myPointOfInterests.size()); }

	private:
		std::vector<std::unique_ptr<State>> myAvailableStates;
		State* myState = nullptr;

		Tga::Vector2f myTargetPosition;

		float myStateTimer = 0.f;

		int myInterestIdx = 0;
		std::vector<Tga::Vector2f> myPointOfInterests;
	};
}
