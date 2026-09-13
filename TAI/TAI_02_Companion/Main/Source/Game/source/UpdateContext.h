#pragma once
#include <vector>

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

struct AIEvent
{
	enum class Type
	{
		PlayerMoved
	};

	Type type;
	std::vector<Tga::Vector2f> pathPositions;
};