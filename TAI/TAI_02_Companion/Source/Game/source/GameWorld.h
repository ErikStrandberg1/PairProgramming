#pragma once
#include <vector>
#include "tge/Engine.h"
#include <tge/drawers/DebugDrawer.h>
#include "tge/drawers/LineDrawer.h"
#include "tge/graphics/Camera.h"
#include <vector>
//AI
#include "Actor.h"
#include "AIEventManager.h"
#include "Controller.h"

namespace Tga
{
	class Texture;
}

static Tga::Vector2f ConvertToPos(Tga::Vector2f aPos);

class Navmesh;

class GameWorld
{
public:
	struct AABB
	{
		Tga::Vector2f min;
		Tga::Vector2f max;

		bool Contains(const Tga::Vector2f& aPosition) const
		{
			return aPosition.x >= min.x &&
				aPosition.x <= max.x &&
				aPosition.y >= min.y &&
				aPosition.y <= max.y;
		}
	};

	GameWorld();
	~GameWorld();

	void Init();
	void Update(const UpdateContext& inputContext);
	void Render();

	Tga::Vector2f GetScreenMin() { return myScreenMin; };
	Tga::Vector2f GetScreenMax() { return myScreenMax; };

	Navmesh* GetNavMesh() { return myNavmesh; }

private:
	void UpdateEnemyPath();
	Tga::Vector2f ConvertToPos(const Tga::Vector2f& aNavmeshPos) const;
	Tga::Vector2f ConvertToNavmesh(const Tga::Vector2f& aWorldPos) const;

	int myLastGoalNode = -1;
	Tga::Vector2f myLastGoalPos{};
	std::vector<Tga::Vector2f> mySmoothPath;

	Tga::Vector2f myResolution{};
	Navmesh* myNavmesh;

	//AI
	Actor* myPlayer;
	Actor* myEnemy;

	std::vector<std::unique_ptr<Actor>> myActors;
	AI::ControllerFactory myControllerFactory;
	AIEventManager myAIEventManager;
	Tga::Texture* myBackgroundTexture;

	Tga::Vector2f myScreenMin;
	Tga::Vector2f myScreenMax;
	Tga::Camera camera;
};
