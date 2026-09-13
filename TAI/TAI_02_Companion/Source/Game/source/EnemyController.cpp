#include "EnemyController.h"

AI::EnemyEventController::EnemyEventController()
{
	AIEventManager::GetInstance().Subscribe(this);
}

AI::EnemyEventController::~EnemyEventController()
{
	AIEventManager::GetInstance().Unsubscribe(this);
}

Tga::Vector2f AI::EnemyEventController::Update(
	const UpdateContext&,
	const Tga::Vector2f& aPosition)
{
	if (!myShouldMove || myPathPositions.empty())
		return {};

	const float arriveThreshold = 0.015f;
	const float stopDistance = 0.06f;

	// Skip past every waypoint we've already reached.
	while (myIndex < (int)myPathPositions.size() &&
		(myPathPositions[myIndex] - aPosition).Length() <= arriveThreshold)
	{
		++myIndex;
	}

	if (myIndex >= (int)myPathPositions.size())
	{
		myShouldMove = false;
		myPathPositions.clear();
		myIndex = 0;
		return {};
	}

	// Don't shove the player around once we've caught up.
	if (myIndex == (int)myPathPositions.size() - 1 &&
		(myPathPositions[myIndex] - aPosition).Length() < stopDistance)
	{
		return {};
	}

	return myPathPositions[myIndex] - aPosition;
}

void AI::EnemyEventController::OnEvent(const AIEvent& aEvent)
{
	if (aEvent.type != AIEvent::Type::PlayerMoved)
		return;

	myPathPositions = aEvent.pathPositions;
	// Index 0 is the companion's own position — start at the next corner.
	myIndex = myPathPositions.size() > 1 ? 1 : 0;
	myShouldMove = !myPathPositions.empty();
}
