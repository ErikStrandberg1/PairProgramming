#include "GameWorld.h"

#include <tge/graphics/GraphicsEngine.h>
#include <tge/graphics/dx11.h>

#include <tge/drawers/SpriteDrawer.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/DebugDrawer.h>
#include <tge/engine.h>
#include "UpdateContext.h"
#include <tge/input/InputManager.h>


using namespace Tga;

GameWorld::GameWorld()
{
}

GameWorld::~GameWorld()
{
}

void GameWorld::Init()
{
	AI::PollingStation& pollingStation =
		AI::PollingStation::GetInstance();

	auto player = std::make_unique<Actor>();

	player->Init(
		"../data/sprites/hacker.png",
		0.2f,
		myControllerFactory.CreateController(AI::eControllerType::ePlayer, AI::eSteeringType::eWander),
		{0.f, 0.5f}
	);
	myPlayer = player.get();
	myActors.push_back(std::move(player));

	int enemyCount = 128;
	for (int i = 0; i < enemyCount; ++i)
	{
		auto enemy = std::make_unique<Actor>();
		Vector2f startPos = {.3f + (float)i / 128, .3f + (float)i / (128.f / 2.f)};

		float speed = .05f + (float)(rand() % 20) / 1000.f;
		enemy->Init(
			"../data/sprites/killerRobo1.png",
			speed,
			myControllerFactory.CreateController(AI::eControllerType::eEnemy, AI::eSteeringType::eFlock),
			startPos
		);

		myActors.push_back(std::move(enemy));
	}

	std::vector<Actor*> tempActors = {};
	tempActors.reserve(myActors.size());
	for (const auto& actor : myActors)
	{
		tempActors.push_back(actor.get());
	}
	pollingStation.Init(std::move(tempActors), myPlayer);
}


void GameWorld::Update(const UpdateContext& context)
{
	for (auto& actor : myActors)
	{
		actor->Update(context);
	}


}

void GameWorld::Render()
{
	auto& engine = *Tga::Engine::GetInstance();

	Tga::Vector2f resolution = Tga::Vector2f((float)Tga::DX11::GetResolution().x, (float)Tga::DX11::GetResolution().y);
	myScreenMin = {0.f, 0.f};
	myScreenMax = {1.0f, 1.0f};

	camera.SetOrtographicProjection(myScreenMin.x, myScreenMax.x, myScreenMin.y, myScreenMax.y, -1.0f, 1.0f);
	engine.GetGraphicsEngine().GetGraphicsStateStack().SetCamera(camera);

	Tga::SpriteDrawer& spriteDrawer(engine.GetGraphicsEngine().GetSpriteDrawer());

	{
		Tga::SpriteSharedData sharedData = {};
		sharedData.myTexture = myBackgroundTexture;

		Tga::Sprite2DInstanceData instanceData = {};
		instanceData.myPivot = {0.0f, 1.0f};
		instanceData.myPosition = {0.0f, 0.0f};
		instanceData.mySize = {1.0f, 1.0f};

		spriteDrawer.Draw(sharedData, instanceData);
	}
	for (auto& actor : myActors)
	{
		actor->Render();
	}
}
