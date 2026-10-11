#include "AnimatedCharacter.h"

#include <algorithm>
#include <tge/animation/Skeleton.h>
#include <tge/model/ModelFactory.h>
#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/LineDrawer.h>
#include <tge/primitives/LinePrimitive.h>
#include "imgui/imgui.h"

using namespace Tga;

static bool ComboFromList(const char* aLabel, const std::vector<std::string>& someItems, int& aIndex)
{
	bool changed = false;
	if (ImGui::BeginCombo(aLabel, someItems[aIndex].c_str()))
	{
		for (int i = 0; i < static_cast<int>(someItems.size()); ++i)
		{
			if (ImGui::Selectable(someItems[i].c_str(), i == aIndex))
			{
				aIndex = i;
				changed = true;
			}
		}
		ImGui::EndCombo();
	}
	return changed;
}

void AnimatedCharacter::Init(const std::string& aModelPath, const std::vector<std::string>& someAnimationPaths)
{
	ModelFactory& modelFactory = ModelFactory::GetInstance();
	myModelInstance = modelFactory.GetAnimatedModelInstance(aModelPath);

	for (const std::string& path : someAnimationPaths)
	{
		AnimationPlayer player = modelFactory.GetAnimationPlayer(path, myModelInstance.GetModel()->GetSkeleton());
		if (!player.IsValid())
		{
			continue;
		}

		player.SetIsLooping(true);
		player.Play();
		myAnimationPlayers.push_back(player);

		size_t start = path.find('@');
		start = start == std::string::npos ? 0 : start + 1;
		size_t end = path.find_last_of('.');
		myAnimationNames.push_back(path.substr(start, end - start));
	}

	for (const Skeleton::Joint& joint : GetSkeleton().joints)
	{
		myJointNames.push_back(joint.name);
	}

	if (myAnimationPlayers.size() > 1)
	{
		myLayerAnimation = 1;
	}

	myBlendMask.resize(GetSkeleton().joints.size(), false);
	SetBlendJoint(0);
}

void AnimatedCharacter::Update(float aDeltaTime)
{
	if (myAnimationPlayers.empty())
	{
		return;
	}

	AnimationPlayer& basePlayer = myAnimationPlayers[myBaseAnimation];
	AnimationPlayer& layerPlayer = myAnimationPlayers[myLayerAnimation];

	basePlayer.Update(aDeltaTime);
	if (myLayerAnimation != myBaseAnimation)
	{
		layerPlayer.Update(aDeltaTime);
	}

	const LocalSpacePose& basePose = basePlayer.GetLocalSpacePose();
	const LocalSpacePose& layerPose = layerPlayer.GetLocalSpacePose();

	myBlendedPose.count = basePose.count;
	for (size_t i = 0; i < myBlendMask.size(); ++i)
	{
		if (myBlendMask[i])
		{
			myBlendedPose.jointTransforms[i] = ScaleRotationTranslationf::Lerp(
				basePose.jointTransforms[i], layerPose.jointTransforms[i], myBlendFactor);
		}
		else
		{
			myBlendedPose.jointTransforms[i] = basePose.jointTransforms[i];
		}
	}

	GetSkeleton().ConvertPoseToModelSpace(myBlendedPose, myModelSpacePose);
	myModelInstance.SetPose(myModelSpacePose);
}

void AnimatedCharacter::DrawSkeleton() const
{
	if (!myShowSkeleton || myAnimationPlayers.empty())
	{
		return;
	}

	const Skeleton& skeleton = GetSkeleton();

	std::vector<Vector3f> from;
	std::vector<Vector3f> to;
	std::vector<Color> colors;

	for (size_t i = 0; i < skeleton.joints.size(); ++i)
	{
		int parent = skeleton.joints[i].parent;
		if (parent < 0)
		{
			continue;
		}

		from.push_back(GetJointWorldTransform(parent).GetPosition());
		to.push_back(GetJointWorldTransform(static_cast<int>(i)).GetPosition());

		Color color = myBlendMask[i] ? Color(1.f, 0.2f, 0.2f, 1.f) : Color(1.f, 1.f, 1.f, 1.f);
		if (i == static_cast<size_t>(myFollowJoint))
		{
			color = Color(1.f, 1.f, 0.f, 1.f);
		}
		colors.push_back(color);
	}

	if (from.empty())
	{
		return;
	}

	LineMultiPrimitive lines;
	lines.colors = colors.data();
	lines.fromPositions = from.data();
	lines.toPositions = to.data();
	lines.count = static_cast<unsigned int>(from.size());

	GraphicsEngine::GetInstance()->GetLineDrawer().Draw(lines);
}

void AnimatedCharacter::DrawImGui()
{
	if (myAnimationPlayers.empty())
	{
		ImGui::Text("No animations loaded");
		return;
	}

	ImGui::Checkbox("Show Skeleton", &myShowSkeleton);
	ImGui::SliderFloat("Blend", &myBlendFactor, 0.f, 1.f);

	ComboFromList("Base Animation", myAnimationNames, myBaseAnimation);
	ComboFromList("Blend Animation", myAnimationNames, myLayerAnimation);

	int blendJoint = myBlendJoint;
	if (ComboFromList("Blend Bone", myJointNames, blendJoint))
	{
		SetBlendJoint(blendJoint);
	}

	ComboFromList("Follow Bone", myJointNames, myFollowJoint);
}

int AnimatedCharacter::FindJoint(const std::string& aName) const
{
	for (int i = 0; i < static_cast<int>(myJointNames.size()); ++i)
	{
		if (myJointNames[i].find(aName) != std::string::npos)
		{
			return i;
		}
	}
	return 0;
}

void AnimatedCharacter::SetBlendJoint(int aJointIndex)
{
	myBlendJoint = aJointIndex;
	std::fill(myBlendMask.begin(), myBlendMask.end(), false);

	if (!myBlendMask.empty())
	{
		BuildBlendMask(aJointIndex);
	}
}

Matrix4x4f AnimatedCharacter::GetJointWorldTransform(int aJointIndex) const
{
	return myModelSpacePose.jointTransforms[aJointIndex] * myModelInstance.GetTransform();
}

void AnimatedCharacter::BuildBlendMask(int aJointIndex)
{
	myBlendMask[aJointIndex] = true;

	for (unsigned int child : GetSkeleton().joints[aJointIndex].children)
	{
		BuildBlendMask(static_cast<int>(child));
	}
}

const Skeleton& AnimatedCharacter::GetSkeleton() const
{
	return *myModelInstance.GetModel()->GetSkeleton();
}
