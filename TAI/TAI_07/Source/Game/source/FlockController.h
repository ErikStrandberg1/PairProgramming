#pragma once
#include "Controller.h"
#include <tge/input/InputManager.h>
#include "PollingStation.h"
#include "UpdateContext.h"

namespace AI
{
	class FlockController : public Controller
	{
	public:
		FlockController(eSteeringType aSteeringType);
		Tga::Vector2f Update(const UpdateContext& updateContext, const UpdateMoveContext& aUpdateMoveContext) override;

		eControllerType GetType() const override { return eControllerType::eEnemy; }

	private:
		float mySeparationRange = 0.04f;
		float myCohesionRange = 0.17f;
		float myVelMatchingRange = 0.12f;

		float mySeparationWeight = 2.5f;
		float myCohesionWeight = 0.45f;
		float myVelMatchingWeight = 0.35f;
		float myWanderWeight = 0.65f;

		float myWanderAngle = 0.f;
	};
}

