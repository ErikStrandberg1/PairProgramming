#pragma once
#include <span>
#include <vector>

#include "Controller.h"
#include "State.h"

struct UpdateContext;

namespace AI
{
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
		bool IsBanditCaptured(const Tga::Vector2f aMyPosition) const;

		int GetCurrentWayPoint() const { return myInterestIdx; }
		std::vector<Tga::Vector2f> GetWayPoints() const { return myPointOfInterests; }
		void NextWayPoint();

		bool TryStartChase(const Tga::Vector2f& aMyPosition);

		bool HasDogFoundBandit() const { return myDogHasFoundBandit; }
		void ClearDogFoundBandit() { myDogHasFoundBandit = false; }
		const Tga::Vector2f& GetDogFoundBanditPos() const { return myDogFoundBanditPos; }

	private:
		bool myDogHasFoundBandit = false;
		Tga::Vector2f myDogFoundBanditPos;

		std::vector<std::unique_ptr<State>> myAvailableStates;
		State* myState = nullptr;

		Tga::Vector2f myTargetPosition;

		float myStateTimer = 0.f;

		int myInterestIdx = 0;
		std::vector<Tga::Vector2f> myPointOfInterests;
	};
}
