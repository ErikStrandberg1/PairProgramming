#include "stdafx.h"
#include "Controller.h"

#include "FlockController.h"
#include "SeekController.h"
#include "SeparationController.h"
#include "UpdateContext.h"
#include "WanderController.h"

namespace AI
{
	Tga::Vector2f Controller::Update([[maybe_unused]] const UpdateContext& updateContext,
	                                 [[maybe_unused]] const UpdateMoveContext& aUpdateMoveContext)
	{
		return Tga::Vector2f{};
	}

	void Controller::OnEvent([[maybe_unused]] const AIEvent& aEvent)
	{
	}

	ControllerFactory::ControllerFactory()
	{
	}

	Controller* ControllerFactory::CreateController(const eControllerType aControllerType, eSteeringType aSteering)
	{
		switch (aControllerType)
		{
			case eControllerType::ePlayer:
			{
				return new WanderController(aSteering);
			}

			case eControllerType::eEnemy:
			{
				if (aSteering == eSteeringType::eSeek)
				{
					return new SeekController(aSteering);
				}
				if (aSteering == eSteeringType::eSeparation)
				{
					return new SeparationController(aSteering);
				}
				if (aSteering == eSteeringType::eFlock)
				{
					return new FlockController(aSteering);
				}
			}
		}
		return nullptr;
	}
}
