#pragma once
#include "Controller.h"
#include <tge/input/InputManager.h>
#include "PollingStation.h"
#include "UpdateContext.h"

struct UpdateContext;

using namespace AI;

namespace AI
{
	class WanderController : public Controller
	{
	public:
		WanderController(eSteeringType aSteeringType);
		Tga::Vector2f Update(const UpdateContext& updateContext, const UpdateMoveContext& aUpdateMoveContext) override;

		eControllerType GetType() const override { return eControllerType::ePlayer; }
		bool ShouldFaceVelocity() const override { return true; }

	private:
		float myWanderAngle = 0.f;

	};
}
