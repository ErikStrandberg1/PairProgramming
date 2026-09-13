#pragma once
#include <memory>
#include <vector>

#include "tge/Engine.h"
#include "tge/drawers/SpriteDrawer.h"
#include "tge/sprite/sprite.h"
#include "tge/texture/TextureManager.h"
#include "tge/graphics/GraphicsEngine.h"
#include "tge/text/Text.h"
#include <tge/drawers/DebugDrawer.h>

#include "InputMapper.h"

enum class StateChangeAction
{
	None,
	StartPlaying,
	ReturnToMainMenu,
	GoBack,
	Quit,
	ShowInGameMenu,
	ShowOption,
};

enum class TimerState
{
	None,
	Enabled,
	Disabled,
	Hidden
};

struct StateUpdateResult
{
	StateChangeAction action = StateChangeAction::None;
};

class State : public GameActionObserver
{
public:
	State()
	{
		Tga::Engine& engine = *Tga::Engine::GetInstance();
		Tga::Vector2f resolution = static_cast<Tga::Vector2f>(engine.GetRenderSize());
		Tga::Vector2f center = resolution / 2;

		mySpriteInstance.myPivot = {.5f, .4f};
		mySpriteInstance.myPosition = center;
		mySpriteInstance.mySizeMultiplier = 2;

		Tga::Text* timerText = new Tga::Text{"Text/arial.ttf", Tga::FontSize_18};
		Tga::Vector2f textPos = {150, resolution.y - 150};
		timerText->SetPosition(textPos);
		timerText->SetText("Time: " + std::to_string(myActiveTimer));

		myTimerText = timerText;
	}

	virtual ~State();

	void Render() const
	{
		Tga::SpriteDrawer& spriteDrawer = Tga::Engine::GetInstance()->GetGraphicsEngine().GetSpriteDrawer();
		spriteDrawer.Draw(mySpriteData, mySpriteInstance);

		if (myTimerState == TimerState::Enabled)
		{
			myTimerText->Render();
		}
	}

	StateChangeAction GetStateAction() const { return myStateAction; }

	void ClearStateAction() { myStateAction = StateChangeAction::None; }

	void UpdateTimer(float aDeltaTime)
	{
		myActiveTimer += aDeltaTime;
		myTimerText->SetText(std::format("Time: {:.1f}", myActiveTimer));
	}

	void SetTimerState(TimerState aTimerState) { myTimerState = aTimerState; }

	void ResetTimer() { myActiveTimer = 0.0f; }

protected:
	Tga::Sprite2DInstanceData mySpriteInstance;
	Tga::SpriteSharedData mySpriteData;

	StateChangeAction myStateAction{};

	Tga::Text* myTimerText;

	TimerState myTimerState = TimerState::Disabled;
	float myActiveTimer = 0.0f;
};

class MenuState : public State
{
public:
	MenuState()
	{
		mySpriteData.myTexture = Tga::Engine::GetInstance()->GetTextureManager().GetTexture("Sprites/Menu.png");
		mySpriteInstance.mySize = mySpriteData.myTexture->CalculateTextureSize();
	}

	~MenuState() override
	{
	}

	void OnAction(GameAction action, ActionEventType type, const AdditionalData& additionalData) override
	{
		if (action == GameAction::StartPlaying)
		{
			myStateAction = StateChangeAction::StartPlaying;
		}
		if (action == GameAction::ShowOption)
		{
			myStateAction = StateChangeAction::ShowOption;
		}
		if (action == GameAction::Quit)
		{
			myStateAction = StateChangeAction::Quit;
		}
		UNREFERENCED_PARAMETER(type);
		UNREFERENCED_PARAMETER(additionalData);
	}
};

class GameState : public State
{
public:
	GameState()
	{
		mySpriteData.myTexture = Tga::Engine::GetInstance()->GetTextureManager().GetTexture("Sprites/InGame.png");
		mySpriteInstance.mySize = mySpriteData.myTexture->CalculateTextureSize();
	}

	~GameState() override
	{
	}

	void OnAction(GameAction action, ActionEventType type, const AdditionalData& additionalData) override
	{
		if (action == GameAction::ShowInGameMenu)
		{
			myStateAction = StateChangeAction::ShowInGameMenu;
		}
		UNREFERENCED_PARAMETER(type);
		UNREFERENCED_PARAMETER(additionalData);
	}
};

class OptionsState : public State
{
public:
	OptionsState()
	{
		mySpriteData.myTexture = Tga::Engine::GetInstance()->GetTextureManager().GetTexture("Sprites/OptionsMenu.png");
		mySpriteInstance.mySize = mySpriteData.myTexture->CalculateTextureSize();
	}

	~OptionsState() override
	{
	}

	void OnAction(GameAction action, ActionEventType type, const AdditionalData& additionalData) override
	{
		if (action == GameAction::GoBack)
		{
			myStateAction = StateChangeAction::GoBack;
		}
		UNREFERENCED_PARAMETER(type);
		UNREFERENCED_PARAMETER(additionalData);
	}
};

class InGameMenu : public State
{
public:
	InGameMenu()
	{
		mySpriteData.myTexture = Tga::Engine::GetInstance()->GetTextureManager().GetTexture("Sprites/InGameMenu.png");
		mySpriteInstance.mySize = mySpriteData.myTexture->CalculateTextureSize();
	}

	~InGameMenu() override
	{
	}

	void OnAction(GameAction action, ActionEventType type, const AdditionalData& additionalData) override
	{
		if (action == GameAction::StartPlaying)
		{
			myStateAction = StateChangeAction::StartPlaying;
		}
		if (action == GameAction::ShowOption)
		{
			myStateAction = StateChangeAction::ShowOption;
		}
		if (action == GameAction::ReturnToMainMenu)
		{
			myStateAction = StateChangeAction::ReturnToMainMenu;
		}
		if (action == GameAction::Quit)
		{
			myStateAction = StateChangeAction::Quit;
		}
		UNREFERENCED_PARAMETER(type);
		UNREFERENCED_PARAMETER(additionalData);
	}
};

class GameStateManager
{
public:
	GameStateManager();
	~GameStateManager();

	void Init();
	void Update(float aTimeDelta);
	void Render();

	State* GetActiveState() const { return myStateStack.empty() ? nullptr : myStateStack.back(); }

	template <class TState>
	State* GetState() const;

	template <class TState>
	State* GetState();

	template <class TState>
	bool IsStateActive() const;

private:
	std::vector<State*> myStateStack;
	std::vector<std::unique_ptr<State>> myStates;
};

template <class TState>
State* GameStateManager::GetState() const
{
	for (const auto& state : myStates)
	{
		if (TState* targetState = dynamic_cast<TState*>(state.get()))
		{
			return targetState;
		}
	}
	return nullptr;
}

template <class TState>
State* GameStateManager::GetState()
{
	for (auto& state : myStates)
	{
		if (TState* targetState = dynamic_cast<TState*>(state.get()))
		{
			return targetState;
		}
	}
	return nullptr;
}

template <class TState>
bool GameStateManager::IsStateActive() const
{
	for (auto* activeState : myStateStack)
	{
		if (dynamic_cast<TState*>(activeState) != nullptr)
		{
			return true;
		}
	}
	return false;
}
