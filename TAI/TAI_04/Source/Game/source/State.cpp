#include "State.h"

#include "Actor.h"

Tga::Vector2f AI::IdleState::Update([[maybe_unused]] AI::GuardController& aGuardController,
                                    [[maybe_unused]] const UpdateContext& aCtx,
                                    [[maybe_unused]] const UpdateMoveContext& aMoveCtx)
{
	return Tga::Vector2f{};
}

Tga::Vector2f AI::ChaseState::Update([[maybe_unused]] AI::GuardController& aGuardController,
                                     [[maybe_unused]] const UpdateContext& aCtx,
                                     [[maybe_unused]] const UpdateMoveContext& aMoveCtx)
{
	return Tga::Vector2f{};
}

AI::PatrolState::PatrolState()
{
	//for (int i = 0; i < 3; ++i)
	//{
	//	float posX = (float)(rand() % 1000) / 1000.f;
	//	float posY = (float)(rand() % 1000) / 1000.f;
	//	myPointOfInterests.emplace_back(posX, posY);
	//}
	myPointOfInterests.emplace_back(0.1f, 0.2f);
	myPointOfInterests.emplace_back(0.1f, 0.2f);
	myPointOfInterests.emplace_back(0.3f, 0.72f);
	myPointOfInterests.emplace_back(0.7f, 0.52f);
}

Tga::Vector2f AI::PatrolState::Update([[maybe_unused]] AI::GuardController& aGuardController,
                                      [[maybe_unused]] const UpdateContext& aCtx,
                                      [[maybe_unused]] const UpdateMoveContext& aMoveCtx)
{
	constexpr float ARRIVE_RADIUS = 0.03f;
	if ((myPointOfInterests[myInterestIdx] - aMoveCtx.pos).LengthSqr() < ARRIVE_RADIUS * ARRIVE_RADIUS)
	{
		myInterestIdx = (myInterestIdx + 1) % static_cast<int>(myPointOfInterests.size());
	}
	return myPointOfInterests[myInterestIdx];
}
