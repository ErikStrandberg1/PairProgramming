#pragma once
#include "Controller.h"
#include <tge/input/InputManager.h>
#include "UpdateContext.h"

using namespace AI;

namespace AI
{
	class PlayerController : public Controller
	{
	public:
		PlayerController();
		~PlayerController() override;
		Tga::Vector2f Update(const UpdateContext& updateContext, const Tga::Vector2f& aPosition) override;
	};

}

	
