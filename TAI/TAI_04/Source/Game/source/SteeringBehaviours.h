#pragma once
#include "UpdateContext.h"

namespace AI::Steering
{
	Tga::Vector2f Seek(const UpdateMoveContext&, Tga::Vector2f target, float maxSpeed, float maxForce);
	Tga::Vector2f Wander(const UpdateMoveContext&, float& wanderAngle, float dt, float maxSpeed, float maxForce,
	                     float rate = 50.f);
	Tga::Vector2f Separation(const UpdateMoveContext&, float aRange, float aMaxForce);
	Tga::Vector2f Cohesion(const UpdateMoveContext&, float aRange, float aMaxSpeed, float aMaxForce);
	Tga::Vector2f VelocityMatching(const UpdateMoveContext&, float aRange, float aMaxForce);
}
