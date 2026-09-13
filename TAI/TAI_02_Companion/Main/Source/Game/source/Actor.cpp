#include "Actor.h"
#include <tge/engine.h>
#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/sprite/sprite.h>
#include <tge/settings/settings.h>
#include <tge/texture/texturemanager.h>
#include "Controller.h"
#include "UpdateContext.h"

#include <assert.h>

Actor::Actor()
{
	myRotation = 0.f;
	mySpeed = 0.f;
	myController = nullptr;
	mySpriteTexture = nullptr;
	myShadowTexture = nullptr;
	myPosition.x = 0.f;
	myPosition.y = 0.f;
}

Actor::Actor(const char* aSpritePath, const float aSpeed, AI::Controller* aAIController, const Tga::Vector2f& aStartPosition)
{
	Init(aSpritePath, aSpeed, aAIController, aStartPosition);
}

Actor::~Actor() {}

void Actor::Init(const char* aSpritePath, const float aSpeed, AI::Controller* aAIController, const Tga::Vector2f& aStartPosition)
{
	auto& engine = *Tga::Engine::GetInstance();
	mySpriteTexture = engine.GetTextureManager().GetTexture(aSpritePath);
	myShadowTexture = engine.GetTextureManager().GetTexture("../data/sprites/blobShadow.png");

	mySpeed = aSpeed;
	myController = aAIController;
	myPosition = aStartPosition;
}


void Actor::Render()
{

	auto& engine = *Tga::Engine::GetInstance();
	Tga::SpriteDrawer& spriteDrawer(engine.GetGraphicsEngine().GetSpriteDrawer());

	{
		Tga::SpriteSharedData sharedData = {};
		sharedData.myTexture = myShadowTexture;

		Tga::Sprite2DInstanceData instanceData = {};
		instanceData.myPivot = { 0.5f, 0.5f };
		instanceData.myPosition = myPosition;
		instanceData.mySize = { 0.1f, 0.1f };

		spriteDrawer.Draw(sharedData, instanceData);
	}
	{
		Tga::SpriteSharedData sharedData = {};
		sharedData.myTexture = mySpriteTexture;

		Tga::Sprite2DInstanceData instanceData = {};
		instanceData.myPivot = { 0.5f, 0.5f };
		instanceData.myPosition = myPosition;
		instanceData.myRotation = myRotation;
		instanceData.mySize = { 0.1f, 0.1f };

		spriteDrawer.Draw(sharedData, instanceData);
	}
}

void Actor::Update(const UpdateContext& updateContext)
{
	if (myController != nullptr)
	{
		Tga::Vector2f direction = myController->Update(updateContext, { myPosition.x, myPosition.y });
		if (!(direction.x == 0.f && direction.y == 0.f)) // Is not zero vector
		{
			direction.Normalize();
			myPosition += direction * mySpeed * updateContext.myDeltaTime;
			myRotation = atan2f(direction.y, direction.x);
		}
	}
}

const Tga::Vector2f& Actor::GetPosition() const
{
	return myPosition;
}

void Actor::SetPosition(const Tga::Vector2f& aPosition)
{
	myPosition = aPosition;
}
