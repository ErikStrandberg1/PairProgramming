#pragma once

#include <vector>
#include <tge/sprite/sprite.h>
#include <tge/graphics/Camera.h>
#include "Actor.h"
#include "Controller.h"
#include "PollingStation.h"
#include "AIEventManager.h"

namespace Tga
{
	class Texture;
	class Text;
}

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
	void RenderWinScreen();

private:
	Actor* myPlayer;
	std::vector<std::unique_ptr<Actor>> myActors;
	std::unique_ptr<AI::PollingStation> myPollingStation;
	AI::ControllerFactory myControllerFactory;
	AIEventManager myAIEventManager;
	Tga::Texture* myBackgroundTexture;

	Tga::Vector2f myScreenMin;
	Tga::Vector2f myScreenMax;
	Tga::Camera camera;

	float myElapsedTime = 0.f;
	std::unique_ptr<Tga::Text> myWinTitle;
	std::unique_ptr<Tga::Text> myWinInfo;
	std::unique_ptr<Tga::Text> myWinHint;
};
