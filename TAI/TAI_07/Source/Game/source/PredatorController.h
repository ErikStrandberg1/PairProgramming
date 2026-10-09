#pragma once
#include "Controller.h"
#include "UpdateContext.h"

namespace AI
{
	enum class PredatorStates
	{
		Wander,
		Hunt,
	};

	class PredatorController : public Controller
	{
	public:
		PredatorController();
		Tga::Vector2f Update(const UpdateContext& updateContext, const UpdateMoveContext& aUpdateMoveContext) override;

		eControllerType GetType() const override { return eControllerType::ePredator; }
		PredatorStates GetState() const { return myState; }
		void SetState(PredatorStates aState);

	private:
		Tga::Vector2f UpdateWander(const UpdateContext& aCtx, const UpdateMoveContext& aMoveCtx);
		Tga::Vector2f UpdateHunt(const UpdateContext& aCtx, const UpdateMoveContext& aMoveCtx);

		PredatorStates myState = PredatorStates::Wander;
		float myWanderAngle = 0.f;
		float myAge = 0.f;
		float myLifeTime = 0.f;
	};
}
