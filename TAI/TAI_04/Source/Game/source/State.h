#pragma once
#include <vector>

#include "UpdateContext.h"
#include "stdafx.h"

namespace AI
{
	class GuardController;

	enum class GuardStates
	{
		Idle,
		Patrol,
		Chase,
	};

	class State
	{
	public:
		State() = default;
		virtual ~State() = default;   

		virtual void EnterState([[maybe_unused]] Tga::Vector2f aPosition)
		{
		}
		virtual Tga::Vector2f Update(AI::GuardController& aGuardController,
		                             const UpdateContext& aCtx,
		                             const UpdateMoveContext& aMoveCtx) = 0;

	protected:
		GuardStates myCurrentState = GuardStates::Idle; 
	};

	class IdleState : public State
	{
	public:
		IdleState() = default;

		Tga::Vector2f Update(AI::GuardController& aGuardController,
		                     const UpdateContext& aCtx,
		                     const UpdateMoveContext& aMoveCtx) override;
	};

	class ChaseState : public State
	{
	public:
		ChaseState() = default;
		Tga::Vector2f Update(AI::GuardController& aGuardController,
		                     const UpdateContext& aCtx,
		                     const UpdateMoveContext& aMoveCtx) override;
	};

	class PatrolState : public State
	{
	public:
		PatrolState();
		Tga::Vector2f Update(AI::GuardController& aGuardController,
		                     const UpdateContext& aCtx,
		                     const UpdateMoveContext& aMoveCtx) override;

	private:
		int myInterestIdx;
		std::vector<Tga::Vector2f> myPointOfInterests;
	};
}
