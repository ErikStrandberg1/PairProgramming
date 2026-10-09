#pragma once
#include "UpdateContext.h"
#include <tge/math/vector.h>
#include <cfloat>
#include <vector>
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

	inline bool IsWithinRange(const Tga::Vector2f& aA, const Tga::Vector2f& aB, float aRange)
	{
		return (aA - aB).LengthSqr() < aRange * aRange;
	}

	inline Tga::Vector2f GetClosest(const std::vector<Tga::Vector2f>& aPoints, const Tga::Vector2f& aPos)
	{
		Tga::Vector2f closest = aPos;
		float closestDistSqr = FLT_MAX;
		for (const auto& point : aPoints)
		{
			const float distSqr = (point - aPos).LengthSqr();
			if (distSqr < closestDistSqr)
			{
				closestDistSqr = distSqr;
				closest = point;
			}
		}
		return closest;
	}

	enum class eControllerType
	{
		ePlayer,
		eEnemy,
		eGuard,
		eBandit,
		eDog,
		eDummy,
		ePrey,
		ePredator,
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
		virtual ~Controller() = default;
		virtual Tga::Vector2f Update(const UpdateContext& updateContext, const UpdateMoveContext& aUpdateMoveContext);
		virtual void OnEvent([[maybe_unused]] const AIEvent& aEvent);

		virtual eControllerType GetType() const = 0;
		virtual bool ShouldFaceVelocity() const { return true; }

		virtual void SetOwner(Actor* aOwner) { myOwner = aOwner; }

		float GetMaxSpeed() const { return myMaxSpeed; }
		float GetMaxForce() const { return myMaxForce; }

	protected:
		float myMaxSpeed = 3;
		float myMaxForce = 1;
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
