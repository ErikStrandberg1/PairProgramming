#pragma once
#include <span>
#include <vector>

#include "Controller.h"
#include "State.h"
#include "BrainTree.h"

struct UpdateContext;

namespace BT = BrainTree;

namespace AI
{
	constexpr float THIRST_THRESHOLD = 5.f;
	constexpr float DRINK_DURATION = 4.f;
	constexpr float SUSPICIOUS_COOLDOWN = 6.f;

	class BehaviourTreeController : public Controller
	{
	public:
		BehaviourTreeController();
		~BehaviourTreeController();
		virtual Tga::Vector2f Update(const UpdateContext& updateContext,
		                             const UpdateMoveContext& aUpdateMoveContext);

		eControllerType GetType() const override { return eControllerType::eDog; }

		float GetThirst() const { return myThirst; }
		void ResetThirst() { myThirst = 0.f; }

		bool IsOnSuspiciousCooldown() const { return mySuspiciousStartCD; }
		void StartSuspiciousCooldown() { mySuspiciousStartCD = true; }

		const Tga::Vector2f& GetFoundBanditPos() const { return myFoundBanditPos; }
		void SetFoundBanditPos(const Tga::Vector2f& aPosition) { myFoundBanditPos = aPosition; }

		void SeekTo(const Tga::Vector2f& aTarget);

		const UpdateContext& GetUpdateContext() const { return myUpdateContext; }
		const UpdateMoveContext& GetMoveContext() const { return myMoveContext; }
		void SetSteering(const Tga::Vector2f& aSteering) { mySteering = aSteering; }

	private:
		float myThirst = 0.f;
		float mySuspiciousTimer = 0.f;
		bool mySuspiciousStartCD = false;

		bool myBanditHiding = false;

		Tga::Vector2f myFoundBanditPos;

		UpdateContext myUpdateContext{};
		UpdateMoveContext myMoveContext{};
		Tga::Vector2f mySteering;
		BT::Node::Ptr myTree;
	};
}
