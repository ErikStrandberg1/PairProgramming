#include "SteeringBehaviours.h"
#include "Controller.h"
#include "PollingStation.h"

Tga::Vector2f AI::Steering::Seek(const UpdateMoveContext& aUpdateMoveContext, Tga::Vector2f target,
                                 float maxSpeed,
                                 float maxForce)
{
	const auto vel = aUpdateMoveContext.vel;
	const auto pos = aUpdateMoveContext.pos;
	const auto targetPos = target; 
	const Tga::Vector2f desired = (targetPos - pos).GetNormalized() * maxSpeed;

	Tga::Vector2f steering = desired - vel;

	if (steering.LengthSqr() > maxForce * maxForce)
	{
		steering = steering.GetNormalized() * maxForce;
	}

	return steering;
}

Tga::Vector2f AI::Steering::Wander(const UpdateMoveContext& aUpdateMoveContext, float& wanderAngle, float dt,
                                   float maxSpeed,
                                   float maxForce, float rate)
{
	const float offset = 150.f;
	const float radius = 100.f;
	const auto vel = aUpdateMoveContext.vel;
	const auto pos = aUpdateMoveContext.pos;
	const auto aDeltaTime = dt;

	Tga::Vector2f heading = {1.f, 0.f};
	if (vel.LengthSqr() > 0.0001f)
	{
		heading = vel.GetNormalized();
	}

	float random = (rand() % 1000 - rand() % 1000) / 1000.f;
	wanderAngle += random * rate * aDeltaTime;

	float angle = std::atan2(heading.y, heading.x) + wanderAngle;
	Tga::Vector2f target = pos + heading * offset
		+ Tga::Vector2f(std::cos(angle), std::sin(angle)) * radius;

	Tga::Vector2f desired = (target - pos).GetNormalized() * maxSpeed;
	Tga::Vector2f steering = desired - vel;

	if (steering.LengthSqr() > maxForce * maxForce)
	{
		steering = steering.GetNormalized() * maxForce;
	}

	return steering;
}

Tga::Vector2f AI::Steering::Separation(const UpdateMoveContext& aUpdateMoveContext,
                                       float aRange,
                                       float aMaxForce)
{
	const float rangeSqr = aRange * aRange;
	const float innerRadius = aRange * 0.5f;
	Tga::Vector2f steering = {};

	for (const Actor* other : PollingStation::GetInstance().GetActors())
	{
		if (other == aUpdateMoveContext.self)
		{
			continue;
		}

		const Tga::Vector2f away = aUpdateMoveContext.pos - other->GetPosition();
		const float distSqr = away.LengthSqr();
		if (distSqr > rangeSqr || distSqr < 0.0000001f)
		{
			continue;
		}

		const float dist = std::sqrt(distSqr);
		const float strength = std::min(1.f, (aRange - dist) / (aRange - innerRadius));
		steering += (away / dist) * strength;
	}

	steering = steering * aMaxForce;
	if (steering.LengthSqr() > aMaxForce * aMaxForce)
	{
		steering = steering.GetNormalized() * aMaxForce;
	}
	return steering;
}

Tga::Vector2f AI::Steering::Cohesion(const UpdateMoveContext& aUpdateMoveContext, float aRange, float aMaxSpeed,
                                     float aMaxForce)
{
	const float rangeSqr = aRange * aRange;
	Tga::Vector2f center = {};
	int count = 0;

	for (const Actor* other : PollingStation::GetInstance().GetActors())
	{
		if (other == aUpdateMoveContext.self)
		{
			continue;
		}
		if ((other->GetPosition() - aUpdateMoveContext.pos).LengthSqr() > rangeSqr)
		{
			continue;
		}
		center += other->GetPosition();
		++count;
	}

	if (count == 0)
	{
		return {};
	}

	center /= static_cast<float>(count);
	const Tga::Vector2f toCenter = center - aUpdateMoveContext.pos;
	const float dist = toCenter.Length();
	if (dist < 0.0001f)
	{
		return {};
	}

	const Tga::Vector2f desired = toCenter.GetNormalized() * aMaxSpeed;
	Tga::Vector2f steering = desired - aUpdateMoveContext.vel;
	if (steering.LengthSqr() > aMaxForce * aMaxForce)
	{
		steering = steering.GetNormalized() * aMaxForce;
	}

	return steering * std::min(dist / aRange, 1.f);
}

Tga::Vector2f AI::Steering::VelocityMatching(const UpdateMoveContext& aUpdateMoveContext, float aRange,
                                             float aMaxForce)
{
	const float rangeSqr = aRange * aRange;
	Tga::Vector2f averageVel = {};
	int count = 0;

	for (const Actor* other : PollingStation::GetInstance().GetActors())
	{
		if (other == aUpdateMoveContext.self)
		{
			continue;
		}
		if ((other->GetPosition() - aUpdateMoveContext.pos).LengthSqr() > rangeSqr)
		{
			continue;
		}
		averageVel += other->GetVelocity();
		++count;
	}

	if (count == 0 || averageVel.LengthSqr() < 0.000001f)
	{
		return {};
	}

	Tga::Vector2f steering = (averageVel.GetNormalized() - aUpdateMoveContext.vel.GetNormalized()) * aMaxForce;
	if (steering.LengthSqr() > aMaxForce * aMaxForce)
	{
		steering = steering.GetNormalized() * aMaxForce;
	}
	return steering;
}


