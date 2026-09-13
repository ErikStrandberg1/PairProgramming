#pragma once
#include <iostream>
#include <vector>

#include "CommonUtilities/InputHandler.h"
#include "tge/math/Vector2.h"

using InputManager = CommonUtilities::InputHandler;

struct InputAction
{
	// Matches Windows Virtual-Key Codes were defined
	// Can add stuff outside that range if desired
	int keyCode;
};

enum class GameAction
{
	None,

	MovePlayerUp,
	MovePlayerDown,
	MovePlayerLeft,
	MovePlayerRight,

	StartPlaying,
	ReturnToMainMenu,
	GoBack,
	Quit,
	ShowInGameMenu,
	ShowOption,
	//....

	SpeedUp,
	Shoot,
	ChangeBullet1,
	ChangeBullet2,

	RotatePlayer,

	Count,
};

enum class ActionEventType
{
	Press,
	Release,
	Held,
	Other,
	None,
};

struct AdditionalData
{
	float amount;
	Tga::Vector2f mousePos;
};

struct GameActionState
{
	ActionEventType event;
	bool isUpOrDown;
	AdditionalData additionalData;
};

class GameActionObserver
{
public:
	virtual void OnAction(GameAction action, ActionEventType type, const AdditionalData& additionalData) = 0;
};

class InputMapper
{
public:
	struct UpdateContext
	{
		InputManager* inputManager;
		float deltaTime;
		std::vector<GameActionObserver*> observer;
	};

	InputMapper();
	~InputMapper();

	void MapAction(InputAction inputAction, GameAction gameAction);

	void RegisterObserver(GameAction action, ActionEventType type, GameActionObserver* observer);
	void RemoveObserver(GameAction action, ActionEventType type, GameActionObserver* observer);

	void Update(const UpdateContext& context);

	//GameActionState GetActionState(GameAction action);

private:
	std::array<std::vector<InputAction>, (size_t)GameAction::Count> myGameToInputActionMapping;

	std::array<std::vector<GameActionObserver*>, (size_t)GameAction::Count> myGameInputToObserverPressed;
	std::array<std::vector<GameActionObserver*>, (size_t)GameAction::Count> myGameInputToObserverReleased;
	std::array<std::vector<GameActionObserver*>, (size_t)GameAction::Count> myGameInputToObserverHeld;

	std::array<std::vector<GameActionObserver*>, (size_t)GameAction::Count> myGameInputToObserverOther;
};
