#include "stdafx.h"
#include "Controller.h"
#include "PlayerController.h"
#include "UpdateContext.h"

namespace AI
{
    ControllerFactory::ControllerFactory()
        : myPollingStation(nullptr)
    {}

    void ControllerFactory::Init(PollingStation* aPollingStation)
    {
        myPollingStation = aPollingStation;
    }

    Controller* ControllerFactory::CreateController(
        const eControllerType aControllerType)
    {
        switch (aControllerType)
        {
            case eControllerType::ePlayer:
                return new PlayerController();

            case eControllerType::ePolling:
                return new EnemyPollController(myPollingStation);

            case eControllerType::eEvent:
                return new EnemyEventController();
        }

        return nullptr;
    }

}
