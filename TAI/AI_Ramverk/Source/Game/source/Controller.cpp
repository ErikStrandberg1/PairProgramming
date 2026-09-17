#include "stdafx.h"
#include "Controller.h"

#include "EnemyController.h"
#include "PlayerController.h"
#include "UpdateContext.h"

namespace AI
{
	Tga::Vector2f Controller::Update(const UpdateContext& updateContext, const UpdateMoveContext& aUpdateMoveContext)
	{
		switch (mySteeringType)
		{
		case eSteeringType::eWander:
		{
			return Wander(updateContext, aUpdateMoveContext);
		}
		case eSteeringType::eSeek:
		{
			return Seek(updateContext, aUpdateMoveContext);
		}
		case eSteeringType::eSeparation:
		{
			return Separation(updateContext, aUpdateMoveContext);
		}
		}
		return Tga::Vector2f{};
	}

	void Controller::OnEvent([[maybe_unused]] const AIEvent& aEvent)
	{
	}

	Tga::Vector2f Controller::Wander(const UpdateContext& updateContext, const UpdateMoveContext& aUpdateMoveContext)
	{
		const float offset = 150.f;
		const float radius = 100.f;
		const float rate = 50.f;
		const auto vel = aUpdateMoveContext.vel;
		const auto pos = aUpdateMoveContext.pos;
		const auto aDeltaTime = updateContext.myDeltaTime;

		Tga::Vector2f heading = { 1.f, 0.f };
		if (vel.LengthSqr() > 0.0001f)
		{
			heading = vel.GetNormalized();
		}

		float random = (rand() % 1000 - rand() % 1000) / 1000.f;
		myWanderAngle += random * rate * aDeltaTime;

		float angle = std::atan2(heading.y, heading.x) + myWanderAngle;
		Tga::Vector2f target = pos + heading * offset
			+ Tga::Vector2f(std::cos(angle), std::sin(angle)) * radius;

		Tga::Vector2f desired = (target - pos).GetNormalized() * myMaxSpeed;
		Tga::Vector2f steering = desired - vel;

		if (steering.LengthSqr() > myMaxForce * myMaxForce)
		{
			steering = steering.GetNormalized() * myMaxForce;
		}

		return steering;
	}

	Tga::Vector2f Controller::Seek([[maybe_unused]] const UpdateContext& updateContext,
		const UpdateMoveContext& aUpdateMoveContext)
	{
		const auto vel = aUpdateMoveContext.vel;
		const auto pos = aUpdateMoveContext.pos;
		const auto targetPos = PollingStation::GetInstance().GetPlayerPosition();
		const Tga::Vector2f desired = (targetPos - pos).GetNormalized() * myMaxSpeed;

		Tga::Vector2f steering = desired - vel;

		if (steering.LengthSqr() > myMaxForce * myMaxForce)
		{
			steering = steering.GetNormalized() * myMaxForce;
		}

		return steering;
	}

	Tga::Vector2f Controller::Separation([[maybe_unused]] const UpdateContext& updateContext,
		const UpdateMoveContext& aUpdateMoveContext) const
	{
		const float threshold = 0.08f; 
		const float decayMultiplier = 0.03f;

		const Tga::Vector2f pos = aUpdateMoveContext.pos;
		Tga::Vector2f steering = {};

		for (Actor* other : PollingStation::GetInstance().GetActors())
		{
			Tga::Vector2f toOther = pos - other->GetPosition();
			const float distSqr = toOther.LengthSqr();
			
			
			if (distSqr < 0.000001f)
			{
				continue;
			}
			if (distSqr > threshold * threshold)
			{
				continue;
			}

			const float strength = std::min(decayMultiplier / distSqr, myMaxForce);
			steering += toOther.GetNormalized() * strength;
		}

		if (steering.LengthSqr() > myMaxForce * myMaxForce)
		{
			steering = steering.GetNormalized() * myMaxForce;
		}

		return steering;
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
				return new PlayerController(aSteering);
			}

			case eControllerType::eEnemy:
			{
				return new EnemyController(aSteering);
			}
		}
		return nullptr;
	}
}
