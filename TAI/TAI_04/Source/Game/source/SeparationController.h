#pragma once
#include "Controller.h"
#include <tge/input/InputManager.h>
#include "PollingStation.h"
#include "UpdateContext.h"

struct UpdateContext;

using namespace AI;

namespace AI
{
	class SeparationController : public Controller
	{
	public:
		SeparationController(eSteeringType aSteeringType);
		Tga::Vector2f Update(const UpdateContext& updateContext, const UpdateMoveContext& aUpdateMoveContext) override;

		eControllerType GetType() const override { return eControllerType::ePlayer; }
		bool ShouldFaceVelocity() const override { return mySteeringType != eSteeringType::eSeparation; }

	private:
	};
}
