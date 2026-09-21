#include "AIEventManager.h"

AIEventManager& AIEventManager::GetInstance()
{
	static AIEventManager instance;
	return instance;
}

void AIEventManager::Subscribe(Controller* aController)
{
	myListeners.push_back(aController);
}

void AIEventManager::Unsubscribe(Controller* aController)
{
	std::erase(myListeners, aController);
}

void AIEventManager::SendEvent(const AIEvent& aEvent)
{
	for (auto listener :myListeners)
	{
		if (listener)
		{
			listener->OnEvent(aEvent);
		}
	}
}

