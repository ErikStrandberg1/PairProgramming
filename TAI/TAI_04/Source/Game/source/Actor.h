#pragma once
#include <tge/math/vector.h>

namespace Tga
{
	class Texture;
}

namespace AI
{
	class Controller;
}

struct UpdateContext;

class Actor
{
public:
	Actor();
	Actor(const char* aSpritePath, const float aSpeed, AI::Controller* aAIController,
	      const Tga::Vector2f& aStartPosition);
	~Actor();

	void Init(const char* aSpritePath, const float aSpeed, AI::Controller* aAIController,
	          const Tga::Vector2f& aStartPosition);
	void Render();
	void Update(const UpdateContext& inputContext);
	const Tga::Vector2f& GetPosition() const;

	AI::Controller* GetController() const;
	const Tga::Vector2f& GetVelocity() const { return myVel; }

private:
	Tga::Vector2f myPosition;
	Tga::Texture* mySpriteTexture;
	Tga::Texture* myShadowTexture;
	AI::Controller* myController;
	float mySpeed;
	float myRotation;

	Tga::Vector2f myVel = 0.f;
	Tga::Vector2f myDirection = {1.f, 0.f};
	Tga::Vector2f mySmoothedSteering = {};
};
