#pragma once
#include <tge/input/InputManager.h>
#include "Controller.h"
#include "UpdateContext.h"

struct UpdateContext;

using namespace AI;

namespace AI
{
	class EnemyController : public Controller
	{
	public:
		EnemyController(eSteeringType aSteeringType);

		eControllerType GetType() const override { return eControllerType::eEnemy; }
		bool ShouldFaceVelocity() const override { return mySteeringType != eSteeringType::eSeparation; } 

	private:
	};
}
