#include "stdafx.h"
#include "GameWorld.h"
#include "PlayerController.h"
#include "UpdateContext.h"

#include <tge/input/InputManager.h>
#include <tge/graphics/DX11.h>

#include "Controller.h"

PlayerController::PlayerController(eSteeringType aSteeringType) 
{
	mySteeringType = aSteeringType;
}
