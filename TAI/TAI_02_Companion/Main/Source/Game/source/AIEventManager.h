#include <tge/math/vector.h>
#include "Actor.h"
#include "UpdateContext.h"
#include <vector>
#include "Controller.h"

using namespace AI;

class AIEventManager
{
public:

	static AIEventManager& GetInstance();

	void Subscribe(Controller* aController);
	void Unsubscribe(Controller* aController);

	void SendEvent(const AIEvent& aEvent);

private:
	std::vector<Controller*> myListeners;

	PlayerOnPC myPlayerOnPc;
};
