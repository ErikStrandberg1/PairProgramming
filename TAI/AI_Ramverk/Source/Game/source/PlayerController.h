#pragma once
#include "Controller.h"
#include <tge/input/InputManager.h>
#include "PollingStation.h"
#include "UpdateContext.h"

struct UpdateContext;

using namespace AI;

namespace AI
{
	class PlayerController : public Controller
	{
	public:
		PlayerController(eSteeringType aSteeringType);
		eControllerType GetType() const override { return eControllerType::ePlayer; }

	private:
	};
}
