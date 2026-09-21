#pragma once
#include "UpdateContext.h"
#include <tge/math/vector.h>

struct UpdateContext;
struct AIEvent;

namespace AI
{
	class PollingStation;

	enum class eControllerType
	{
		ePlayer,
		eEnemy,
	};

	enum class eSteeringType
	{
		eSeek,
		eWander,
		eSeparation,
	};

	class Controller
	{
	public:
		virtual Tga::Vector2f Update(const UpdateContext& updateContext,
		                             const UpdateMoveContext& aUpdateMoveContext);

		virtual void OnEvent([[maybe_unused]] const AIEvent& aEvent);

		virtual eControllerType GetType() const = 0;
		virtual bool ShouldFaceVelocity() const { return true; }

	protected:
		eSteeringType mySteeringType = eSteeringType::eSeek;
		float myMaxSpeed = 3;
		float myMaxForce = 1;
	};

	class ControllerFactory
	{
	public:
		ControllerFactory();
		Controller* CreateController(const eControllerType aControllerType, eSteeringType aSteering);

	private:
	};
}
