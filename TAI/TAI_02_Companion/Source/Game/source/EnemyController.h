#pragma once
#include "Controller.h"
#include <tge/input/InputManager.h>
#include "UpdateContext.h"
#include "AIEventManager.h"

using namespace AI;

namespace AI
{
	class EnemyEventController : public Controller
	{
	public:
		EnemyEventController();
		~EnemyEventController() override;
		Tga::Vector2f Update(const UpdateContext& updateContext, const Tga::Vector2f& aPosition) override;
		void OnEvent(const AIEvent& aEvent) override;
	private:
		std::vector<Tga::Vector2f> myPathPositions;
		int myIndex = 0;
		bool myShouldMove = false;
	};
}

	
