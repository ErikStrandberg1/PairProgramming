#pragma once
#include <tge/math/vector2.h>

namespace Tga
{
	class InputManager;
}

class GameWorld;

struct UpdateContext
{
	float myDeltaTime;
	const Tga::InputManager* myInputManager;
	GameWorld* myGameWold;
};

class Actor;

struct UpdateMoveContext
{
	Tga::Vector2f pos;
	Tga::Vector2f vel;
	const Actor* self = nullptr; 
};

struct AIEvent
{
	enum class Type
	{
		ComputerActivated
	};

	Type myType;
	Tga::Vector2f myPosition;
};