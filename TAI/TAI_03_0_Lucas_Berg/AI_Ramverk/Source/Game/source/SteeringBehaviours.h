#pragma once
#include "PollingStation.h"
#include "UpdateContext.h"

namespace AI::Steering
{
	Tga::Vector2f Seek(const UpdateMoveContext&, Tga::Vector2f target, float maxSpeed, float maxForce);
	Tga::Vector2f Wander(const UpdateMoveContext&, float& wanderAngle, float dt, float maxSpeed, float maxForce);
	Tga::Vector2f Separation(const UpdateMoveContext&, float aRange, float aMaxForce);
}
