#include "GameWorld.h"

#include <tge/graphics/GraphicsEngine.h>
#include <tge/graphics/dx11.h>

#include <tge/drawers/SpriteDrawer.h>
#include <tge/texture/TextureManager.h>
#include <tge/engine.h>
#include "UpdateContext.h"
#include <tge/input/InputManager.h>
#include <imgui/imgui.h>

#include <cstdio>
#include <ctime>

using namespace Tga;

namespace
{
	constexpr int START_PREY_COUNT = 20;
	constexpr int START_PREDATOR_COUNT = 3;
	constexpr float PREY_SPEED = 0.09f;
	constexpr float PREDATOR_SPEED = 0.13f;
	constexpr float FENCE_THICKNESS = 0.002f;
}

GameWorld::GameWorld()
{
}

GameWorld::~GameWorld()
{
}

void GameWorld::Init()
{
	srand(static_cast<unsigned>(time(nullptr)));

	for (int i = 0; i < START_PREY_COUNT; ++i)
	{
		SpawnPrey(GetRandomPositionInsideFence());
	}
	for (int i = 0; i < START_PREDATOR_COUNT; ++i)
	{
		SpawnPredator(GetRandomPositionInsideFence());
	}

	UpdatePollingStation();
}

void GameWorld::Update(const UpdateContext& context)
{
	for (auto& actor : myActors)
	{
		actor->Update(context);
	}

	RemoveDeadActors();
	UpdatePollingStation();

	myWorldDirector.Update(*this, context.myDeltaTime);
}

void GameWorld::Render()
{
	auto& engine = *Tga::Engine::GetInstance();

	myScreenMin = {0.f, 0.f};
	myScreenMax = {1.0f, 1.0f};

	camera.SetOrtographicProjection(myScreenMin.x, myScreenMax.x, myScreenMin.y, myScreenMax.y, -1.0f, 1.0f);
	engine.GetGraphicsEngine().GetGraphicsStateStack().SetCamera(camera);

	RenderFence();

	for (auto& actor : myActors)
	{
		actor->Render();
	}

#ifndef _RETAIL
	RenderImGui();
#endif
}

void GameWorld::SpawnPrey(const Tga::Vector2f& aPosition)
{
	auto prey = std::make_unique<Actor>();
	prey->Init(
		"../data/sprites/sheep.png",
		PREY_SPEED,
		myControllerFactory.CreateController(AI::eControllerType::ePrey, AI::eSteeringType::eWander),
		aPosition
	);
	prey->SetColor({0.1f, 0.9f, 0.1f, 1.f});
	myActors.push_back(std::move(prey));
}

void GameWorld::SpawnPredator(const Tga::Vector2f& aPosition)
{
	auto predator = std::make_unique<Actor>();
	predator->Init(
		"../data/sprites/wolf.png",
		PREDATOR_SPEED,
		myControllerFactory.CreateController(AI::eControllerType::ePredator, AI::eSteeringType::eSeek),
		aPosition
	);
	predator->SetColor({1.f, 0.1f, 0.1f, 1.f});
	myActors.push_back(std::move(predator));
}

Tga::Vector2f GameWorld::GetRandomPositionInsideFence() const
{
	return {AI::RandomRange(myFenceMin.x, myFenceMax.x), AI::RandomRange(myFenceMin.y, myFenceMax.y)};
}

void GameWorld::RemoveDeadActors()
{
	std::erase_if(myActors, [](const std::unique_ptr<Actor>& aActor) { return aActor->IsDead(); });
}

void GameWorld::UpdatePollingStation()
{
	std::vector<Actor*> preys;
	std::vector<Actor*> predators;

	for (auto& actor : myActors)
	{
		if (actor->GetController() == nullptr)
		{
			continue;
		}
		if (actor->GetController()->GetType() == AI::eControllerType::ePrey)
		{
			preys.push_back(actor.get());
		}
		else if (actor->GetController()->GetType() == AI::eControllerType::ePredator)
		{
			predators.push_back(actor.get());
		}
	}

	AI::PollingStation& pollingStation = AI::PollingStation::GetInstance();
	pollingStation.SetPreys(std::move(preys));
	pollingStation.SetPredators(std::move(predators));
}

void GameWorld::RenderFence()
{
	auto& engine = *Tga::Engine::GetInstance();
	Tga::SpriteDrawer& spriteDrawer(engine.GetGraphicsEngine().GetSpriteDrawer());

	Tga::SpriteSharedData sharedData = {};
	sharedData.myTexture = engine.GetTextureManager().GetWhiteSquareTexture();

	const Tga::Vector2f center = (myFenceMin + myFenceMax) * 0.5f;
	const Tga::Vector2f size = myFenceMax - myFenceMin;

	Tga::Sprite2DInstanceData instanceData = {};
	instanceData.myPivot = {0.5f, 0.5f};
	instanceData.myColor = {1.f, 0.f, 0.f, 1.f};

	instanceData.mySize = {size.x + FENCE_THICKNESS, FENCE_THICKNESS};
	instanceData.myPosition = {center.x, myFenceMin.y};
	spriteDrawer.Draw(sharedData, instanceData);
	instanceData.myPosition = {center.x, myFenceMax.y};
	spriteDrawer.Draw(sharedData, instanceData);

	instanceData.mySize = {FENCE_THICKNESS, size.y + FENCE_THICKNESS};
	instanceData.myPosition = {myFenceMin.x, center.y};
	spriteDrawer.Draw(sharedData, instanceData);
	instanceData.myPosition = {myFenceMax.x, center.y};
	spriteDrawer.Draw(sharedData, instanceData);
}

void GameWorld::RenderImGui()
{
	AI::PollingStation& pollingStation = AI::PollingStation::GetInstance();

	ImGui::SetNextWindowSize(ImVec2(300.f, 430.f), ImGuiCond_FirstUseEver);
	ImGui::Begin("GameWorld ImGui");

	ImGui::Text("Nr of prey: %d", static_cast<int>(pollingStation.GetPreys().size()));
	ImGui::Text("Nr of predators: %d", static_cast<int>(pollingStation.GetPredators().size()));
	ImGui::NewLine();

	ImGui::Text("Director state: %s", myWorldDirector.GetStateName());
	ImGui::NewLine();

	if (ImGui::Button("Debug spawn prey"))
	{
		SpawnPrey(GetRandomPositionInsideFence());
	}
	if (ImGui::Button("Debug spawn predator"))
	{
		SpawnPredator(GetRandomPositionInsideFence());
	}
	ImGui::NewLine();

	ImGui::Text("Increase prey threshold: %d", myWorldDirector.GetIncreasePreyThreshold());
	ImGui::Text("Decrease prey threshold: %d", myWorldDirector.GetDecreasePreyThreshold());
	ImGui::NewLine();

	ImGui::Text("Prey reproduction interval: %.2f", myWorldDirector.GetPreyReproductionInterval());

	ImGui::End();
}
