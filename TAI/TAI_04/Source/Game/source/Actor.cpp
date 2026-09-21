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

#include "GameWorld.h"
#include "tge/math/CommonMath.h"

static constexpr float STOP_SPEED = 0.01f;
static constexpr float MIN_STEERING_FORCE = 0.001f;
static constexpr float VELOCITY_DAMPING = 0.97f;
static constexpr float ACTOR_SIZE = 0.035f;
static constexpr float MIN_ROTATION_FORCE = 0.02f;

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

Actor::Actor(const char* aSpritePath, const float aSpeed, AI::Controller* aAIController,
             const Tga::Vector2f& aStartPosition)
{
	Init(aSpritePath, aSpeed, aAIController, aStartPosition);
}

Actor::~Actor()
{
}

void Actor::Init(const char* aSpritePath, const float aSpeed, AI::Controller* aAIController,
                 const Tga::Vector2f& aStartPosition)
{
	auto& engine = *Tga::Engine::GetInstance();
	mySpriteTexture = engine.GetTextureManager().GetTexture(aSpritePath);
	myShadowTexture = engine.GetTextureManager().GetTexture("../data/sprites/blobShadow.png");

	mySpeed = aSpeed;
	myController = aAIController;
	myPosition = aStartPosition;

	const float startAngle = (float)rand() / (float)RAND_MAX * 2.f * FMath::Pi;
	myVel = Tga::Vector2f(std::cos(startAngle), std::sin(startAngle)) * (aSpeed * 0.5f);
}


void Actor::Render()
{
	auto& engine = *Tga::Engine::GetInstance();
	Tga::SpriteDrawer& spriteDrawer(engine.GetGraphicsEngine().GetSpriteDrawer());

	{
		Tga::SpriteSharedData sharedData = {};
		sharedData.myTexture = myShadowTexture;

		Tga::Sprite2DInstanceData instanceData = {};
		instanceData.myPivot = {0.5f, 0.5f};
		instanceData.myPosition = myPosition;
		instanceData.mySize = {ACTOR_SIZE, ACTOR_SIZE};

		spriteDrawer.Draw(sharedData, instanceData);
	}
	{
		Tga::SpriteSharedData sharedData = {};
		sharedData.myTexture = mySpriteTexture;

		Tga::Sprite2DInstanceData instanceData = {};
		instanceData.myPivot = {0.5f, 0.5f};
		instanceData.myPosition = myPosition;
		instanceData.myRotation = myRotation;
		instanceData.mySize = {ACTOR_SIZE, ACTOR_SIZE};

		spriteDrawer.Draw(sharedData, instanceData);
	}
}

void Actor::Update(const UpdateContext& updateContext)
{
	if (myController == nullptr)
	{
		return;
	}

	const Tga::Vector2f steeringForce = myController->Update(updateContext, {myPosition, myVel, this});
	UpdateMovement(updateContext, steeringForce);
	ScreenWrap(updateContext);
	
}

const Tga::Vector2f& Actor::GetPosition() const
{
	return myPosition;
}

AI::Controller* Actor::GetController() const
{
	if (myController)
	{
		return myController;
	}
	return nullptr;
}

void Actor::ScreenWrap(const UpdateContext& aUpdateCtx)
{
	const Tga::Vector2f screenMax = aUpdateCtx.myGameWold->GetScreenMax();

	if (myPosition.x < 0.f)
	{
		myPosition.x += screenMax.x;
	}
	else if (myPosition.x > screenMax.x)
	{
		myPosition.x -= screenMax.x;
	}
	if (myPosition.y < 0.f)
	{
		myPosition.y += screenMax.y;
	}
	else if (myPosition.y > screenMax.y)
	{
		myPosition.y -= screenMax.y;
	}
}

void Actor::UpdateMovement(const UpdateContext& aUpdateCtx, Tga::Vector2f aSteeringForce)
{
	auto steeringForce = aSteeringForce;
	auto dt = aUpdateCtx.myDeltaTime;
	
	myVel += steeringForce * dt;

	if (steeringForce.LengthSqr() < MIN_STEERING_FORCE * MIN_STEERING_FORCE)
	{
		myVel *= VELOCITY_DAMPING;
		if (myVel.LengthSqr() < STOP_SPEED * STOP_SPEED)
		{
			myVel = {};
		}
	}

	const float lengthSqr = myVel.LengthSqr();
	if (lengthSqr > mySpeed * mySpeed)
	{
		myVel = myVel * (mySpeed / std::sqrt(lengthSqr));
	}

	myPosition += myVel * dt;

	const float steeringSmoothing = 4.f;
	mySmoothedSteering += (steeringForce - mySmoothedSteering) * std::min(1.f, steeringSmoothing * dt);

	const float turnSpeed = 6.f;
	if (mySmoothedSteering.LengthSqr() > MIN_ROTATION_FORCE * MIN_ROTATION_FORCE)
	{
		const float targetRot = std::atan2(mySmoothedSteering.y, mySmoothedSteering.x);
		const float diffRot = std::remainder(targetRot - myRotation, 2.f * FMath::Pi);
		myRotation += diffRot * std::min(1.f, turnSpeed * dt);
	}
}
