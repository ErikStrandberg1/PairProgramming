#pragma once
#include <vector>
#include "tge/Engine.h"
#include <tge/drawers/DebugDrawer.h>
#include "GameStateManager.h"
#include "InputMapper.h"
#include "Grid.h"
#include "tge/drawers/LineDrawer.h"
#include "GridTypes.h"
#include "tge/graphics/Camera.h"
#include <vector>
//AI
#include "Actor.h"
#include "Controller.h"
#include "AIEventManager.h"

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
	std::vector<int> FindPath(int startNode, int endNode);
	std::vector<Tga::Vector2f> PathToPositions(const std::vector<int>& indices);
private:

	GameStateManager myGameStateManager{};
	Tga::Vector2f myResolution{};

	Navmesh* myNavmesh;
	Tga::Vector2f myStartPosition;
	Tga::Vector2f myEndPosition;
	int myStartNode = -1;
	int myEndNode = -1;
	std::vector<int> myPath;
	bool myMouseWasDown = false;

	Tga::Vector2f myTargetMousePos;
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