#include "BehaviourTreeController.h"

#include "AIEventManager.h"
#include "GameWorld.h"
#include "PollingStation.h"
#include "SteeringBehaviours.h"
#include "tge/texture/TextureManager.h"

namespace AI
{
	namespace
	{
		constexpr float FOUND_BANDIT_RADIUS = 0.06f;
		constexpr float GUARD_REACHED_RADIUS = 0.06f;
		constexpr float SPOT_REACHED_RADIUS = 0.03f;
		constexpr float SAME_SPOT_RADIUS = 0.001f;
		constexpr float BRAKE_STRENGTH = 5.f;

		class DogLeaf : public BT::Leaf
		{
		public:
			DogLeaf(BehaviourTreeController* aController) : myController(aController)
			{
			}

		protected:
			BehaviourTreeController* myController;
		};

		class IsThirsty : public DogLeaf
		{
		public:
			using DogLeaf::DogLeaf;

			Status update() override
			{
				return myController->GetThirst() >= THIRST_THRESHOLD ? Status::Success : Status::Failure;
			}
		};

		class FindWaterSpot : public DogLeaf
		{
		public:
			using DogLeaf::DogLeaf;

			Status update() override
			{
				const Tga::Vector2f pos = myController->GetMoveContext().pos;
				const Tga::Vector2f closest = GetClosest(PollingStation::GetInstance().GetWaterSpots(), pos);
				myController->SeekTo(closest);
				return IsWithinRange(pos, closest, SPOT_REACHED_RADIUS) ? Status::Success : Status::Running;
			}
		};

		class DrinkWater : public DogLeaf
		{
		public:
			using DogLeaf::DogLeaf;

			void initialize() override
			{
				myTimer = 0.f;
			}

			Status update() override
			{
				myTimer += myController->GetUpdateContext().myDeltaTime;
				myController->SetSteering(myController->GetMoveContext().vel * -BRAKE_STRENGTH);

				if (myTimer >= DRINK_DURATION)
				{
					myController->ResetThirst();
					return Status::Success;
				}
				return Status::Running;
			}

		private:
			float myTimer = 0.f;
		};

		class Wander : public DogLeaf
		{
		public:
			using DogLeaf::DogLeaf;

			Status update() override
			{
				myController->SetSteering(Steering::Wander(myController->GetMoveContext(), myWanderAngle,
				                                           myController->GetUpdateContext().myDeltaTime,
				                                           myController->GetMaxSpeed(), myController->GetMaxForce()));
				return Status::Success;
			}

		private:
			float myWanderAngle = 0.f;
		};

		class IsSuspicious : public DogLeaf
		{
		public:
			using DogLeaf::DogLeaf;

			Status update() override
			{
				if (myController->IsOnSuspiciousCooldown())
				{
					return Status::Failure;
				}

				Tga::Vector2f hidingSpot;
				if (!PollingStation::GetInstance().IsBanditHiding(&hidingSpot))
				{
					return Status::Failure;
				}
				myController->SetFoundBanditPos(hidingSpot);

				return Status::Success;
			}
		};

		class FindBandit : public DogLeaf
		{
		public:
			using DogLeaf::DogLeaf;

			Status update() override
			{
				const Tga::Vector2f bushPos = myController->GetFoundBanditPos();

				Tga::Vector2f currentSpot;
				if (!PollingStation::GetInstance().IsBanditHiding(&currentSpot)
					|| !IsWithinRange(currentSpot, bushPos, SAME_SPOT_RADIUS))
				{
					return Status::Failure;
				}

				myController->SeekTo(bushPos);
				return IsWithinRange(myController->GetMoveContext().pos, bushPos, FOUND_BANDIT_RADIUS)
					       ? Status::Success
					       : Status::Running;


			}
		};

		class FindGuard : public DogLeaf
		{
		public:
			using DogLeaf::DogLeaf;

			Status update() override
			{
				const Tga::Vector2f guardPos = PollingStation::GetInstance().GetGuardPosition();
				myController->SeekTo(guardPos);

				if (!IsWithinRange(myController->GetMoveContext().pos, guardPos, GUARD_REACHED_RADIUS))
				{
					return Status::Running;
				}

				AIEvent event;
				event.myType = AIEvent::Type::DogFoundGuard;
				event.myPosition = myController->GetFoundBanditPos();
				AIEventManager::GetInstance().SendEvent(event);

				return Status::Success;
			}
		};

		class ShowHidingSpot : public DogLeaf
		{
		public:
			using DogLeaf::DogLeaf;

			Status update() override
			{
				const Tga::Vector2f& spot = myController->GetFoundBanditPos();
				myController->SeekTo(spot);

				if (!IsWithinRange(myController->GetMoveContext().pos, spot, SPOT_REACHED_RADIUS))
				{
					return Status::Running;
				}

				myController->StartSuspiciousCooldown();
				return Status::Success;
			}
		};
	}
}

AI::BehaviourTreeController::BehaviourTreeController()
{
	myTree = BT::Builder()
	         .composite<BT::Selector>()
	         .composite<BT::Sequence>()
	         .leaf<IsSuspicious>(this)
	         .leaf<FindBandit>(this)
	         .leaf<FindGuard>(this)
	         .leaf<ShowHidingSpot>(this)
	         .end()
	         .composite<BT::Sequence>()
	         .leaf<IsThirsty>(this)
	         .leaf<FindWaterSpot>(this)
	         .leaf<DrinkWater>(this)
	         .end()
	         .leaf<Wander>(this)
	         .end()
	         .build();
}

AI::BehaviourTreeController::~BehaviourTreeController()
{
}

Tga::Vector2f AI::BehaviourTreeController::Update(const UpdateContext& updateContext,
                                                  const UpdateMoveContext& aUpdateMoveContext)
{
	myUpdateContext = updateContext;
	myMoveContext = aUpdateMoveContext;
	mySteering = {};
	myThirst += updateContext.myDeltaTime;
	myTree->tick();
	if (mySuspiciousStartCD)
	{
		mySuspiciousTimer += updateContext.myDeltaTime;
		if (mySuspiciousTimer >= SUSPICIOUS_COOLDOWN)
		{
			mySuspiciousTimer = 0.f;
			mySuspiciousStartCD = false;
		}
	}


	return mySteering;
}

void AI::BehaviourTreeController::SeekTo(const Tga::Vector2f& aTarget)
{
	mySteering = Steering::Seek(myMoveContext, aTarget, myMaxSpeed, myMaxForce);
}
