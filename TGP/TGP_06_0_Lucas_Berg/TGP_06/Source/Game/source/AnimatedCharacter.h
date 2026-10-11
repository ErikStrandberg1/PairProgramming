#pragma once
#include <string>
#include <vector>
#include <tge/model/AnimatedModelInstance.h>
#include <tge/animation/AnimationPlayer.h>
#include <tge/animation/Pose.h>

namespace Tga
{
	struct Skeleton;
}

class AnimatedCharacter
{
public:
	void Init(const std::string& aModelPath, const std::vector<std::string>& someAnimationPaths);
	void Update(float aDeltaTime);
	void DrawSkeleton() const;
	void DrawImGui();

	int FindJoint(const std::string& aName) const;
	void SetBlendJoint(int aJointIndex);
	void SetFollowJoint(int aJointIndex) { myFollowJoint = aJointIndex; }

	int GetFollowJoint() const { return myFollowJoint; }
	Tga::Matrix4x4f GetJointWorldTransform(int aJointIndex) const;
	Tga::AnimatedModelInstance& GetModelInstance() { return myModelInstance; }

	void BuildBlendMask(int aJointIndex);
	const Tga::Skeleton& GetSkeleton() const;

private:
	Tga::AnimatedModelInstance myModelInstance;
	std::vector<Tga::AnimationPlayer> myAnimationPlayers;
	std::vector<std::string> myAnimationNames;
	std::vector<std::string> myJointNames;
	std::vector<bool> myBlendMask;

	Tga::LocalSpacePose myBlendedPose = {};
	Tga::ModelSpacePose myModelSpacePose = {};

	int myBaseAnimation = 0;
	int myLayerAnimation = 0;
	int myBlendJoint = 0;
	int myFollowJoint = 0;
	float myBlendFactor = 1.f;
	bool myShowSkeleton = true;
};
