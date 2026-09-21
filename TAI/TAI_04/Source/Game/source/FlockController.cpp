#include "FlockController.h"
#include "SteeringBehaviours.h"

AI::FlockController::FlockController(eSteeringType aSteeringType)
{
	mySteeringType = aSteeringType;
	myMaxForce = 0.3f;
} 

Tga::Vector2f AI::FlockController::Update(const UpdateContext& updateContext,
                                          const UpdateMoveContext& aUpdateMoveContext)
{
	Tga::Vector2f steering =
		Steering::Separation(aUpdateMoveContext, mySeparationRange, myMaxForce) * mySeparationWeight +
		Steering::Cohesion(aUpdateMoveContext, myCohesionRange, myMaxSpeed, myMaxForce) * myCohesionWeight +
		Steering::VelocityMatching(aUpdateMoveContext, myVelMatchingRange, myMaxForce) * myVelMatchingWeight +
		Steering::Wander(aUpdateMoveContext, myWanderAngle, updateContext.myDeltaTime, myMaxSpeed, myMaxForce, 3.f) *
		myWanderWeight;

	if (steering.LengthSqr() > myMaxForce * myMaxForce)
	{
		steering = steering.GetNormalized() * myMaxForce;
	}
	return steering;
}
