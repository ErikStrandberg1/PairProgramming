#pragma once
#include "UpdateContext.h"

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
		Tga::Vector2f Wander(const UpdateContext& updateContext, const UpdateMoveContext& aUpdateMoveContext);
		Tga::Vector2f Seek(const UpdateContext& updateContext, const UpdateMoveContext& aUpdateMoveContext);
		Tga::Vector2f Separation(const UpdateContext& updateContext, const UpdateMoveContext& aUpdateMoveContext) const;

	protected:
		Tga::Vector2f myTargetPosition;
		eSteeringType mySteeringType = eSteeringType::eSeek;
		float myMaxSpeed = 3;
		float myMaxForce = 1;
		float myWanderAngle = 0.f;
	};

	class ControllerFactory
	{
	public:
		ControllerFactory();
		Controller* CreateController(const eControllerType aControllerType, eSteeringType aSteering);

	private:
	};
}
