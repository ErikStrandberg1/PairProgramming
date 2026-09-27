#pragma once
#include "UpdateContext.h"
#include <tge/math/vector.h>
#include <cstdlib>

struct UpdateContext;
struct AIEvent;
class Actor;

namespace AI
{
	class PollingStation;

	inline float RandomRange(float aMin, float aMax)
	{
		return aMin + (aMax - aMin) * ((float)rand() / (float)RAND_MAX);
	}

	enum class eControllerType
	{
		ePlayer,
		eEnemy,
		eGuard,
		eBandit,
		eDummy,
	};

	enum class eSteeringType
	{
		eSeek,
		eWander,
		eSeparation,
		eFlock,
	};

	class Controller
	{
	public:
		virtual Tga::Vector2f Update(const UpdateContext& updateContext,
		                             const UpdateMoveContext& aUpdateMoveContext);

		virtual void OnEvent([[maybe_unused]] const AIEvent& aEvent);

		virtual eControllerType GetType() const = 0;
		virtual bool ShouldFaceVelocity() const { return true; }

		virtual void SetOwner(Actor* aOwner) { myOwner = aOwner; }

		float myMaxSpeed = 3;
		float myMaxForce = 1;

	protected:
		eSteeringType mySteeringType = eSteeringType::eSeek;
		Actor* myOwner = nullptr;
	};

	class ControllerFactory
	{
	public:
		ControllerFactory();
		Controller* CreateController(const eControllerType aControllerType, eSteeringType aSteering);

	private:
	};
}
