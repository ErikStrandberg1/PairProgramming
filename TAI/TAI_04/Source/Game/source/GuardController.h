#pragma once
#include <vector>

#include "Controller.h"
#include "State.h"

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

		eControllerType GetType() const override { return eControllerType::ePlayer; };

	private:
		std::vector<std::unique_ptr<State>> myAvailableStates;
		State* myState = nullptr;

		Tga::Vector2f myTargetPosition;

	};
}
