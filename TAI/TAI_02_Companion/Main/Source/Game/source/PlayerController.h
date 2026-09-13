#pragma once
#include "Controller.h"
#include <tge/input/InputManager.h>
//#include "PollingStation.h"
#include "UpdateContext.h"

using namespace AI;


namespace AI
{
	class PlayerController : public Controller
	{
	public:
		PlayerController();
		Tga::Vector2f Update(const UpdateContext& updateContext, const Tga::Vector2f& aPosition) override;
	};
	
	class EnemyPollController : public Controller
	{
	public:
		EnemyPollController(PollingStation* aPollingStation);
		Tga::Vector2f Update(const UpdateContext& updateContext, const Tga::Vector2f& aPosition) override;
	private:
		PollingStation* myPollingStation;
		
	};

	class EnemyEventController : public Controller
	{
	public:
		EnemyEventController();
		Tga::Vector2f Update(const UpdateContext& updateContext, const Tga::Vector2f& aPosition) override;
		void OnEvent(const AIEvent& aEvent) override;
	private:
		Tga::Vector2f myTargetPos;

		Tga::Vector2f myStartPosition;
		Tga::Vector2f myEndPosition;
		int myStartNode = -1;
		int myEndNode = -1;
		std::vector<int> myPath;

		Tga::Vector2f myCurrentNodePos;
		int myIndex = 0;

		std::vector<Tga::Vector2f> myPathPositions;

		bool myShouldMove;
	};

}

	
