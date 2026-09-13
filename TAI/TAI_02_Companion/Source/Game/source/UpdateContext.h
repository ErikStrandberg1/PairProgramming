#pragma once
#include <vector>
#include <tge/math/Vector.h>

namespace Tga
{
	class InputManager;
}

class GameWorld;

struct UpdateContext
{
	float deltaTime;
	const Tga::InputManager* inputManager;
	GameWorld* gameWorld;
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
