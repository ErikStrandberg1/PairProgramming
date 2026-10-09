#pragma once
#include "Controller.h"
#include "UpdateContext.h"

namespace AI
{
	enum class PreyStates
	{
		Wander,
		Flee,
	};

	class PreyController : public Controller
	{
	public:
		PreyController();
		Tga::Vector2f Update(const UpdateContext& updateContext, const UpdateMoveContext& aUpdateMoveContext) override;

		eControllerType GetType() const override { return eControllerType::ePrey; }
		PreyStates GetState() const { return myState; }

	private:
		Tga::Vector2f UpdateWander(const UpdateContext& aCtx, const UpdateMoveContext& aMoveCtx);
		Tga::Vector2f UpdateFlee(const UpdateMoveContext& aMoveCtx);

		PreyStates myState = PreyStates::Wander;
		float myWanderAngle = 0.f;
	};
}
