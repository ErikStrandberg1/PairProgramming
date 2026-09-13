#include "stdafx.h"
#include "GameWorld.h"
#include "PlayerController.h"
#include "UpdateContext.h"

#include <tge/input/InputManager.h>
#include <tge/graphics/DX11.h>

#include "Controller.h"
#include "navmesh.h"

AI::PlayerController::PlayerController() {}

Tga::Vector2f AI::PlayerController::Update(const UpdateContext& updateContext, const Tga::Vector2f& aPosition)
{
	if (updateContext.myInputManager->IsKeyPressed(VK_LBUTTON))
	{
		Tga::Vector2ui resolution = Tga::DX11::GetResolution();

		float x = static_cast<float>(updateContext.myInputManager->GetMousePosition().myX)/resolution.x;
		float y = static_cast<float>(updateContext.myInputManager->GetMousePosition().myY)/resolution.y;

		Tga::Vector2f screenMin = updateContext.myGameWold->GetScreenMin();
		Tga::Vector2f screenMax = updateContext.myGameWold->GetScreenMax();

		myTargetPosition.x = screenMin.x + screenMax.x * x;
		myTargetPosition.y = screenMin.y + screenMax.y * (1.0f - y);
	}
	
	if ((myTargetPosition - aPosition).Length() > 0.01f)
	{
		return myTargetPosition - aPosition;
	}
	
	return Tga::Vector2f();





}

AI::EnemyPollController::EnemyPollController(PollingStation* aPollingStation) : myPollingStation(aPollingStation)
{
}

Tga::Vector2f AI::EnemyPollController::Update([[maybe_unused]]const UpdateContext& updateContext, [[maybe_unused]] const Tga::Vector2f& aPosition)
{
	//if (myPollingStation->isPlayerOnComputer() == PlayerOnPC::PC1)
	//{
	//	return myPollingStation->GetPC1Position() - aPosition;
	//}
	//if (myPollingStation->isPlayerOnComputer() == PlayerOnPC::PC2)
	//{
	//	return myPollingStation->GetPC2Position() - aPosition;
	//}
	//if (myPollingStation->isPlayerOnComputer() == PlayerOnPC::PC3)
	//{
	//	return myPollingStation->GetPC3Position() - aPosition;
	//}
	return {0.f, 0.f};
		
}

AI::EnemyEventController::EnemyEventController()
{
	    AIEventManager::GetInstance().Subscribe(this);
}

Tga::Vector2f AI::EnemyEventController::Update(
	const UpdateContext&,
	const Tga::Vector2f& aPosition)
{
	if (!myShouldMove || myPathPositions.empty())
		return {};

	if (myIndex >= myPathPositions.size())
	{
		myShouldMove = false;
		myPathPositions.clear();
		myIndex = 0;
		return {};
	}

	Tga::Vector2f direction =
		myPathPositions[myIndex] - aPosition;

	if (direction.Length() <= 0.01f)
	{
		++myIndex;
		return {};
	}

	return direction;
}

void AI::EnemyEventController::OnEvent(const AIEvent& aEvent)
{
	if (aEvent.type == AIEvent::Type::PlayerMoved)
	{
		myPathPositions.clear();
		myPathPositions = aEvent.pathPositions;
		myShouldMove = true;
		myIndex = 0;

		std::cout << "Received path:\n";

		for (const auto& pos : myPathPositions)
		{
			std::cout << pos.x << ", " << pos.y << "\n";
		}
	}
}

