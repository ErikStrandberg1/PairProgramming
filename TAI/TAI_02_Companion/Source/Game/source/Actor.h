#pragma once
#include <tge/math/vector.h>

enum class PlayerOnPC
{
	None,
	PC1,
	PC2,
	PC3,
};



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
	Actor(const char* aSpritePath, const float aSpeed, AI::Controller* aAIController, const Tga::Vector2f& aStartPosition);
	~Actor();

	void Init(const char* aSpritePath, const float aSpeed, AI::Controller* aAIController, const Tga::Vector2f& aStartPosition);
	void Render();
	void Update(const UpdateContext& inputContext);
	const Tga::Vector2f& GetPosition() const;
	void SetPosition(const Tga::Vector2f&);

	AI::Controller* GetController() const { return myController; }

private:
	Tga::Vector2f myPosition;
	Tga::Texture* mySpriteTexture;
	Tga::Texture* myShadowTexture;
	AI::Controller*	myController;
	float mySpeed;
	float myRotation;
};



