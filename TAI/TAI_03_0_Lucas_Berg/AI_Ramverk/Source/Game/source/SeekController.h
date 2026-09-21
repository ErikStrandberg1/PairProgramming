#pragma once
#include <tge/input/InputManager.h>
#include "Controller.h"
#include "UpdateContext.h"

struct UpdateContext;

using namespace AI;

namespace AI
{
	class SeekController : public Controller
	{
	public:
		SeekController(eSteeringType aSteeringType);
		Tga::Vector2f Update(const UpdateContext& updateContext, const UpdateMoveContext& aUpdateMoveContext) override;

		eControllerType GetType() const override { return eControllerType::eEnemy; }
		bool ShouldFaceVelocity() const override { return true; }

	private:
		Tga::Vector2f myTargetPosition;

	};
}
