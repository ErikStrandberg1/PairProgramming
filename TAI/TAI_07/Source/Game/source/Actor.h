#pragma once
#include <tge/math/vector.h>
#include <tge/math/color.h>

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
	static constexpr float DEFAULT_SIZE = 0.025f;

	Actor();
	Actor(const char* aSpritePath, const float aSpeed, AI::Controller* aAIController,
	      const Tga::Vector2f& aStartPosition);
	~Actor();

	void Init(const char* aSpritePath, const float aSpeed, AI::Controller* aAIController,
	          const Tga::Vector2f& aStartPosition);
	void Render();
	virtual void Update(const UpdateContext& inputContext);
	const Tga::Vector2f& GetPosition() const;
	void Teleport(const Tga::Vector2f& aPosition);
	void SetSize(float aSize) { mySize = aSize; }
	void SetColor(const Tga::Color& aColor) { myColor = aColor; }

	void Kill() { myIsDead = true; }
	bool IsDead() const { return myIsDead; }

	AI::Controller* GetController() const;
	const Tga::Vector2f& GetVelocity() const { return myVel; }

protected:
	void ScreenWrap(const UpdateContext& aUpdateCtx);
	void KeepInsideFence(const UpdateContext& aUpdateCtx);
	void UpdateMovement(const UpdateContext& aUpdateCtx, Tga::Vector2f aSteeringForce);
	Tga::Vector2f myPosition;
	Tga::Texture* mySpriteTexture;
	Tga::Texture* myShadowTexture;
	AI::Controller* myController;
	float mySpeed;
	float myRotation;
	float mySize = DEFAULT_SIZE;
	Tga::Color myColor = {1.f, 1.f, 1.f, 1.f};
	bool myIsDead = false;

	Tga::Vector2f myVel = 0.f;
	Tga::Vector2f myDirection = {1.f, 0.f};
	Tga::Vector2f mySmoothedSteering = {};
};
