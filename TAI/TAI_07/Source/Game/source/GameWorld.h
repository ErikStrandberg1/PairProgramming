#pragma once

#include <vector>
#include <tge/sprite/sprite.h>
#include <tge/graphics/Camera.h>
#include "Actor.h"
#include "Controller.h"
#include "PollingStation.h"
#include "AIEventManager.h"
#include "WorldDirector.h"

class GameWorld
{
public:
	GameWorld();
	~GameWorld();

	void Init();
	void Update(const UpdateContext& inputContext);

	void Render();
	Tga::Vector2f GetScreenMin() { return myScreenMin; };
	Tga::Vector2f GetScreenMax() { return myScreenMax; };
	Tga::Vector2f GetFenceMin() const { return myFenceMin; }
	Tga::Vector2f GetFenceMax() const { return myFenceMax; }

	void SpawnPrey(const Tga::Vector2f& aPosition);
	void SpawnPredator(const Tga::Vector2f& aPosition);
	Tga::Vector2f GetRandomPositionInsideFence() const;

private:
	void RemoveDeadActors();
	void UpdatePollingStation();
	void RenderFence();
	void RenderImGui();

	std::vector<std::unique_ptr<Actor>> myActors;
	AI::ControllerFactory myControllerFactory;
	AI::WorldDirector myWorldDirector;

	Tga::Vector2f myScreenMin;
	Tga::Vector2f myScreenMax;
	Tga::Vector2f myFenceMin = {0.08f, 0.14f};
	Tga::Vector2f myFenceMax = {0.92f, 0.86f};
	Tga::Camera camera;
};
