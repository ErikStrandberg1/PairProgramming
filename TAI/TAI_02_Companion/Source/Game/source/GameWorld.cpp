#include "stdafx.h"

#include "GameWorld.h"
#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/texture/TextureManager.h>
#include "tge\input\InputManager.h"
#include "navmesh.h"

#include <tge/graphics/dx11.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/DebugDrawer.h>
#include <tge/engine.h>
#include "UpdateContext.h"

#include <queue>
#include <cmath>
#include <functional>
#include <limits>
#include <algorithm>

GameWorld::GameWorld()
{
	myNavmesh = nullptr;
}

GameWorld::~GameWorld()
{
	delete myNavmesh;
}

void GameWorld::Init()
{
	myNavmesh = new Navmesh();
	myNavmesh->Init("navmesh/navmesh.obj");

	auto player = std::make_unique<Actor>();
	player->Init(
		"../data/sprites/hacker.png",
		0.3f,
		myControllerFactory.CreateController(AI::eControllerType::ePlayer),
		{0.f, 0.5f}
	);
	myPlayer = player.get();
	myPlayer->SetPosition(ConvertToPos(myNavmesh->GetNodes()[46].myCenter));

	myActors.push_back(std::move(player));

	auto enemy1 = std::make_unique<Actor>();
	enemy1->Init(
		"../data/sprites/killerRobo2.png",
		0.10f,
		myControllerFactory.CreateController(AI::eControllerType::eEvent),
		{.7f, .7f}
	);
	myEnemy = enemy1.get();
	enemy1.get()->SetPosition(ConvertToPos(myNavmesh->GetNodes()[10].myCenter));

	myActors.push_back(std::move(enemy1));
}

void GameWorld::Update(const UpdateContext& context)
{
	for (auto& actor : myActors)
	{
		actor->Update(context);
	}

	UpdateEnemyPath();
}

void GameWorld::UpdateEnemyPath()
{
	if (myNavmesh == nullptr || myPlayer == nullptr || myEnemy == nullptr)
	{
		return;
	}

	const Tga::Vector2f playerNav = ConvertToNavmesh(myPlayer->GetPosition());
	const Tga::Vector2f enemyNav = ConvertToNavmesh(myEnemy->GetPosition());

	// If the player is off the mesh, aim for the nearest point
	int goalNode = -1;
	const Tga::Vector2f goalPos = myNavmesh->GetClosestPointOnMesh(playerNav, goalNode);
	if (goalNode == -1)
	{
		return;
	}

	int startNode = -1;
	const Tga::Vector2f startPos = myNavmesh->GetClosestPointOnMesh(enemyNav, startNode);
	if (startNode == -1)
	{
		return;
	}

	// Only repath when the goal actually moved
	if (goalNode == myLastGoalNode && (goalPos - myLastGoalPos).Length() < 10.f)
	{
		return;
	}

	myLastGoalNode = goalNode;
	myLastGoalPos = goalPos;

	const std::vector<int> nodePath = myNavmesh->FindPath(startNode, goalNode);
	if (nodePath.empty())
	{
		return;
	}

	float spriteLength = std::fmax((float)myEnemy->mySpriteTexture->myImageSize.x,
	                               (float)myEnemy->mySpriteTexture->myImageSize.y);
	float radius = spriteLength / 2.f;
	mySmoothPath = myNavmesh->SmoothPath(nodePath, startPos, goalPos, radius);

	AIEvent event{};
	event.type = AIEvent::Type::PlayerMoved;
	for (const Tga::Vector2f& point : mySmoothPath)
	{
		event.pathPositions.push_back(ConvertToPos(point));
	}

	AIEventManager::GetInstance().SendEvent(event);
}

void GameWorld::Render()
{
	const auto& engine = *Tga::Engine::GetInstance();
	Tga::DebugDrawer& debugDrawer = engine.GetDebugDrawer();

	myNavmesh->RenderNavmesh(debugDrawer);

	for (int i = 0; i + 1 < (int)mySmoothPath.size(); ++i)
	{
		debugDrawer.DrawLine(mySmoothPath[i], mySmoothPath[i + 1], Tga::Color(1.f, 1.f, 0.f, 1.f));
		debugDrawer.DrawCircle(mySmoothPath[i], 4.f, Tga::Color(1.f, 0.5f, 0.f, 1.f));
	}

	myScreenMin = {0.f, 0.f};
	myScreenMax = {1.0f, 1.0f};

	camera.SetOrtographicProjection(myScreenMin.x, myScreenMax.x, myScreenMin.y, myScreenMax.y, -1.0f, 1.0f);
	engine.GetGraphicsEngine().GetGraphicsStateStack().SetCamera(camera);

	for (auto& actor : myActors)
	{
		actor->Render();
	}
}

Tga::Vector2f GameWorld::ConvertToPos(const Tga::Vector2f& aNavmeshPos) const
{
	const auto& engine = *Tga::Engine::GetInstance();
	return {
		aNavmeshPos.x / engine.GetRenderSize().x,
		aNavmeshPos.y / engine.GetRenderSize().y
	};
}

Tga::Vector2f GameWorld::ConvertToNavmesh(const Tga::Vector2f& aWorldPos) const
{
	const auto& engine = *Tga::Engine::GetInstance();
	return {
		aWorldPos.x * engine.GetRenderSize().x,
		aWorldPos.y * engine.GetRenderSize().y
	};
}
