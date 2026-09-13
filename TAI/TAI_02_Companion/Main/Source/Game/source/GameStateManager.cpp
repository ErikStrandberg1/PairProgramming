#include "GameStateManager.h"

State::~State()
{
}

GameStateManager::GameStateManager()
{
}

GameStateManager::~GameStateManager()
{
}

void GameStateManager::Init()
{
	myStates.push_back(std::make_unique<MenuState>());
	myStates.push_back(std::make_unique<InGameMenu>());
	myStates.push_back(std::make_unique<GameState>());
	myStates.push_back(std::make_unique<OptionsState>());

	myStateStack.push_back(GetState<MenuState>());
}

void GameStateManager::Update(float aTimeDelta)
{
	if (myStateStack.empty())
	{
		PostQuitMessage(0);
	}

	for (auto& state : myStates)
	{
		if (state.get() == myStateStack.back())
		{
			myStateStack.back()->SetTimerState(TimerState::Enabled);
			myStateStack.back()->UpdateTimer(aTimeDelta);
		}
		else
		{
			bool hideState = false;

			for (auto stateToHide : myStateStack)
			{
				if (stateToHide == state.get())
				{
					stateToHide->SetTimerState(TimerState::Hidden);
					hideState = true;
					break;
				}
			}
			if (!hideState)
			{
				state.get()->ResetTimer();
			}
		}
	}

	auto* active = myStateStack.back();
	auto stateAction = active->GetStateAction();

	switch (stateAction)
	{
		case StateChangeAction::StartPlaying:
			myStateStack.clear();
			myStateStack.push_back(GetState<GameState>());
			break;

		case StateChangeAction::ShowOption:
			myStateStack.push_back(GetState<OptionsState>());
			break;

		case StateChangeAction::GoBack:
			std::erase(myStateStack, GetState<OptionsState>());
			break;

		case StateChangeAction::Quit:
			myStateStack.clear();
			break;

		case StateChangeAction::ShowInGameMenu:
			myStateStack.push_back(GetState<InGameMenu>());
			break;

		case StateChangeAction::ReturnToMainMenu:
			myStateStack.clear();
			myStateStack.push_back(GetState<MenuState>());
			break;
	}

	active->ClearStateAction();
}

void GameStateManager::Render()
{
	for (auto state : myStateStack)
	{
		state->Render();
	}
}
