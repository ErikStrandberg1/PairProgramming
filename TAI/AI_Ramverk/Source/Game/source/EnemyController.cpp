#include "EnemyController.h"
#include "AIEventManager.h"
#include "PollingStation.h"

EnemyController::EnemyController(eSteeringType aSteeringType)
{
	AIEventManager::GetInstance().Subscribe(this);
	mySteeringType = aSteeringType;
}

