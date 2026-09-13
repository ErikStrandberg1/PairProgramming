#include "InputMapper.h"

#include "tge/input/InputManager.h"

InputMapper::InputMapper()
{
}

InputMapper::~InputMapper()
{
	for (auto pressObservers : myGameInputToObserverPressed)
	{
		pressObservers.clear();
	}

	for (auto releaseObservers : myGameInputToObserverReleased)
	{
		releaseObservers.clear();
	}

	for (auto heldObservers : myGameInputToObserverHeld)
	{
		heldObservers.clear();
	}
}

void InputMapper::MapAction(InputAction inputAction, GameAction gameAction)
{
	myGameToInputActionMapping[static_cast<size_t>(gameAction)].push_back(inputAction);
}

void InputMapper::RegisterObserver(GameAction action, ActionEventType type, GameActionObserver* observer)
{
	const size_t index = static_cast<size_t>(action);

	switch (type)
	{
		case ActionEventType::Press:
			{
				myGameInputToObserverPressed[index].push_back(observer);
				break;
			}
		case ActionEventType::Release:
			{
				myGameInputToObserverReleased[index].push_back(observer);
				break;
			}
		case ActionEventType::Held:
			{
				myGameInputToObserverHeld[index].push_back(observer);
				break;
			}
		case ActionEventType::Other:
			{
				myGameInputToObserverOther[index].push_back(observer);
				break;
			}
		case ActionEventType::None:
			{
				break;
			}
	}
}

void InputMapper::RemoveObserver(GameAction action, ActionEventType type, GameActionObserver* observer)
{
	const size_t index = static_cast<size_t>(action);

	std::vector<GameActionObserver*> v = myGameInputToObserverPressed[index];

	if (type == ActionEventType::Release)
	{
		v = myGameInputToObserverReleased[index];
	}
	if (type == ActionEventType::Held)
	{
		v = myGameInputToObserverHeld[index];
	}

	std::erase_if(v, [observer](const GameActionObserver* aObserver)
	{
		return observer == aObserver;
	});
}

void InputMapper::Update(const UpdateContext& context)
{
	for (int i = 0; i < (size_t)GameAction::Count; i++)
	{
		bool isPressed = false;
		bool isReleased = false;
		bool isHeld = false;

		bool isMouseMoving = false;

		for (InputAction action : myGameToInputActionMapping[i])
		{
			if (context.inputManager->IsKeyDown(action.keyCode))
			{
				isHeld = true;
			}

			if (context.inputManager->IsKeyReleased(action.keyCode))
			{
				isReleased = true;
			}

			if (context.inputManager->IsKeyPressed(action.keyCode))
			{
				isPressed = true;
			}


			// check in input manager
		}

		if (isHeld)
		{
			for (GameActionObserver* observer : myGameInputToObserverHeld[i])
			{
				for (auto obs : context.observer)
				{
					if (observer == obs)
					{
						observer->OnAction((GameAction)i, ActionEventType::Held, {});
					}
				}
			}
		}

		if (isReleased)
		{
			for (GameActionObserver* observer : myGameInputToObserverReleased[i])
			{
				for (auto obs : context.observer)
				{
					if (observer == obs)
					{
						observer->OnAction((GameAction)i, ActionEventType::Release, {});
					}
				}
			}
		}

		if (isPressed)
		{
			for (GameActionObserver* observer : myGameInputToObserverPressed[i])
			{
				for (auto obs : context.observer)
				{
					if (observer == obs)
					{
						observer->OnAction((GameAction)i, ActionEventType::Press, {});
					}
				}
			}
			// call observers
		}

		if (context.inputManager->GetMouseDelta().x != 0 || context.inputManager->GetMouseDelta().y != 0)
		{
			isMouseMoving = true;
		}

		if (isMouseMoving)
		{
			Tga::Vector2f mousePos = {
				static_cast<float>(context.inputManager->GetMousePosition().x),
				static_cast<float>(context.inputManager->GetMousePosition().y)
			};

			for (GameActionObserver* observer : myGameInputToObserverOther[i])
			{
				for (auto obs : context.observer)
				{
					if (observer == obs)
					{
						observer->OnAction((GameAction)i, ActionEventType::Other, {0.f, mousePos});
					}
				}
			}
		}
	}
	context.inputManager->UpdateInput();
}
