#include "SteeringBehaviours.h"

Tga::Vector2f AI::Steering::Seek(const UpdateMoveContext& aUpdateMoveContext, [[maybe_unused]] Tga::Vector2f target,
                                 float maxSpeed,
                                 float maxForce)
{
	const auto vel = aUpdateMoveContext.vel;
	const auto pos = aUpdateMoveContext.pos;
	const auto targetPos = PollingStation::GetInstance().GetPlayerPosition();
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
                                   float maxForce)
{
	const float offset = 150.f;
	const float radius = 100.f;
	const float rate = 50.f;
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
                                       [[maybe_unused]] float aRange,
                                       float aMaxForce)
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

		const float strength = std::min(decayMultiplier / distSqr, aMaxForce);
		steering += toOther.GetNormalized() * strength;
	}

	if (steering.LengthSqr() > aMaxForce * aMaxForce)
	{
		steering = steering.GetNormalized() * aMaxForce;
	}

	return steering;
}
