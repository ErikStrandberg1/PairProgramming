#include "stdafx.h"
#include "GameWorld.h"
#include "PlayerController.h"
#include "UpdateContext.h"

#include <tge/input/InputManager.h>
#include <tge/graphics/DX11.h>

#include "Controller.h"
#include "navmesh.h"

AI::PlayerController::PlayerController() {}

AI::PlayerController::~PlayerController()
{
}

Tga::Vector2f AI::PlayerController::Update(const UpdateContext& updateContext, const Tga::Vector2f& aPosition)
{
	if (updateContext.inputManager->IsKeyPressed(VK_LBUTTON))
	{
		Tga::Vector2ui resolution = Tga::DX11::GetResolution();

		float x = static_cast<float>(updateContext.inputManager->GetMousePosition().myX) / resolution.x;
		float y = static_cast<float>(updateContext.inputManager->GetMousePosition().myY) / resolution.y;

		Tga::Vector2f screenMin = updateContext.gameWorld->GetScreenMin();
		Tga::Vector2f screenMax = updateContext.gameWorld->GetScreenMax();

		myTargetPosition.x = screenMin.x + screenMax.x * x;
		myTargetPosition.y = screenMin.y + screenMax.y * (1.0f - y);
	}
	
	if ((myTargetPosition - aPosition).Length() > 0.01f)
	{
		return myTargetPosition - aPosition;
	}
	
	return Tga::Vector2f();
}
