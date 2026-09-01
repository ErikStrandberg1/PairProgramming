#include "GameWorld.h"

#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/DebugDrawer.h>
#include <tge/application.h>

using namespace Tga;

GameWorld::GameWorld()
{
}

GameWorld::~GameWorld()
{
}

void GameWorld::Init()
{
	Tga::Application& application = *Tga::Application::GetInstance();

	Tga::Vector2ui intResolution = application.GetRenderSize();
	Tga::Vector2f resolution = { (float)intResolution.x, (float)intResolution.y };
	{
		mySharedData.texture = GraphicsEngine::GetInstance()->GetTextureManager().GetTexture("Sprites/tge_logo_w.dds");
		myTGELogoInstance.pivot = { 0.5f, 0.5f };
		myTGELogoInstance.position = Tga::Vector2f{ 0.5f, 0.5f }*resolution;
		myTGELogoInstance.size = Tga::Vector2f{ 0.75f, 0.75f }*resolution.y;
		myTGELogoInstance.color = Tga::Color(1, 1, 1, 1);
	}
}

void GameWorld::Update(float aTimeDelta)
{
	UNREFERENCED_PARAMETER(aTimeDelta);
}

void GameWorld::Render()
{
	auto& graphicsEngine = *Tga::GraphicsEngine::GetInstance();

	Tga::SpriteDrawer& spriteDrawer(graphicsEngine.GetSpriteDrawer());
	// Game update
	{
		spriteDrawer.Draw(mySharedData, myTGELogoInstance);
	}
	// Debug draw pivot
#ifndef _RETAIL
	{
		Tga::DebugDrawer& dbg = graphicsEngine.GetDebugDrawer();
		Tga::Color c1 = myTGELogoInstance.color;
		dbg.DrawCircle(myTGELogoInstance.position, 5.f, (c1.myR + c1.myG + c1.myB) / 3 > 0.3f ? Tga::Color(0, 0, 0, 1) : Tga::Color(1, 1, 1, 1));
	}
#endif

}