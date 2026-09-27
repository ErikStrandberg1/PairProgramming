#include "GameWorld.h"

#include <tge/graphics/GraphicsEngine.h>
#include <tge/graphics/dx11.h>

#include <tge/drawers/SpriteDrawer.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/DebugDrawer.h>
#include <tge/engine.h>
#include "UpdateContext.h"
#include <tge/input/InputManager.h>
#include <tge/text/text.h>

#include <cstdio>
#include <ctime>

#include "BanditController.h"


using namespace Tga;

GameWorld::GameWorld()
{
}

GameWorld::~GameWorld()
{
}

void GameWorld::Init()
{
	srand(static_cast<unsigned>(time(nullptr)));

	std::vector<Tga::Vector2f> hidingSpots = {
		{0.3f, 0.1f},
		{0.35f, 0.84f},
		{0.4f, 0.3f},
		{0.55f, 0.65f},
		{0.6f, 0.8f},
		{0.75f, 0.25f},
		{0.8f, 0.50f}
	};
	for (auto& spot : hidingSpots)
	{
		spot += {AI::RandomRange(-0.04f, 0.04f), AI::RandomRange(-0.04f, 0.04f)};
	}

	AI::PollingStation& pollingStation =
		AI::PollingStation::GetInstance();

	auto player = std::make_unique<Actor>();

	player->Init(
		"../data/sprites/hacker.png",
		0.2f,
		myControllerFactory.CreateController(AI::eControllerType::eDummy, AI::eSteeringType::eWander),
		{0.f, -0.5f}
	);
	myPlayer = player.get();
	myActors.push_back(std::move(player));

	auto guardActor = std::make_unique<Actor>();
	guardActor->Init(
		"../data/sprites/killerRobo1.png",
		0.24f,
		myControllerFactory.CreateController(AI::eControllerType::eGuard, AI::eSteeringType::eWander),
		{0.1f, 0.5f}
	);
	myActors.push_back(std::move(guardActor));


	auto banditActor = std::make_unique<Actor>();
	banditActor->Init(
		"../data/sprites/bandit.png",
		0.13f,
		myControllerFactory.CreateController(AI::eControllerType::eBandit, AI::eSteeringType::eWander),
		{0.9f, 0.5f}
	);
	myActors.push_back(std::move(banditActor));

	int computerSpots = (int)hidingSpots.size();
	for (int i = 0; i < computerSpots; ++i)
	{
		auto computer = std::make_unique<Actor>();
		Vector2f startPos = hidingSpots[i];

		float speed = 0;
		computer->Init(
			"../data/sprites/bush.png",
			speed,
			myControllerFactory.CreateController(AI::eControllerType::eDummy, AI::eSteeringType::eWander),
			startPos
		);
		computer->SetSize(Actor::DEFAULT_SIZE * 2.f);

		myActors.push_back(std::move(computer));
	}

	std::vector<Actor*> tempActors = {};
	tempActors.reserve(myActors.size());
	for (const auto& actor : myActors)
	{
		tempActors.push_back(actor.get());
	}
	pollingStation.Init(std::move(tempActors), myPlayer, hidingSpots);

	myWinTitle = std::make_unique<Tga::Text>("Text/arial.ttf", Tga::FontSize_48);
	myWinTitle->SetColor({1.f, 0.85f, 0.2f, 1.f});
	myWinTitle->SetText("The bandit has won!");

	myWinInfo = std::make_unique<Tga::Text>("Text/arial.ttf", Tga::FontSize_24);
	myWinInfo->SetColor({1.f, 1.f, 1.f, 1.f});

	myWinHint = std::make_unique<Tga::Text>("Text/arial.ttf", Tga::FontSize_18);
	myWinHint->SetColor({0.75f, 0.75f, 0.75f, 1.f});
	myWinHint->SetText("Press 'ESC' to quit");
}


void GameWorld::Update(const UpdateContext& context)
{
	if (AI::PollingStation::GetInstance().HasBanditEscaped())
	{
		if (context.myInputManager->IsKeyPressed(VK_ESCAPE))
		{
			PostQuitMessage(0);
		}
		return;
	}

	myElapsedTime += context.myDeltaTime;

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

	if (AI::PollingStation::GetInstance().HasBanditEscaped())
	{
		RenderWinScreen();
	}
}

void GameWorld::RenderWinScreen()
{
	auto& engine = *Tga::Engine::GetInstance();
	Tga::SpriteDrawer& spriteDrawer(engine.GetGraphicsEngine().GetSpriteDrawer());

	{
		Tga::SpriteSharedData sharedData = {};
		sharedData.myTexture = engine.GetTextureManager().GetWhiteSquareTexture();

		Tga::Sprite2DInstanceData instanceData = {};
		instanceData.myPivot = {0.5f, 0.5f};
		instanceData.myPosition = {0.5f, 0.5f};
		instanceData.mySize = {1.0f, 1.0f};
		instanceData.myColor = {0.f, 0.f, 0.f, 0.65f};

		spriteDrawer.Draw(sharedData, instanceData);
	}

	const int captures = AI::PollingStation::GetInstance().GetBanditCaptures();
	std::string information = std::format("Reached the goal in {:.1f} s  -  caught {} time{}",
	                                      myElapsedTime, captures, captures == 1 ? "" : "s");
	myWinInfo->SetText(information);

	Tga::GraphicsStateStack& graphicsStateStack = engine.GetGraphicsEngine().GetGraphicsStateStack();
	graphicsStateStack.Push();
	graphicsStateStack.SetDefaultCamera();

	const Tga::Vector2f resolution = {(float)Tga::DX11::GetResolution().x, (float)Tga::DX11::GetResolution().y};

	auto renderCentered = [&resolution](Tga::Text& aText, float aHeightRatio)
	{
		aText.SetPosition({resolution.x * 0.5f - aText.GetWidth() * 0.5f, resolution.y * aHeightRatio});
		aText.Render();
	};
	renderCentered(*myWinTitle, 0.58f);
	renderCentered(*myWinInfo, 0.48f);
	renderCentered(*myWinHint, 0.38f);

	graphicsStateStack.Pop();
}
