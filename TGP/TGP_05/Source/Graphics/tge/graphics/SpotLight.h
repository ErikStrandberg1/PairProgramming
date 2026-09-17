// tge/graphics/SpotLight.h
#pragma once
#include <tge/math/color.h>
#include <tge/render/RenderCommon.h>
#include <tge/math/Matrix.h>

namespace Tga
{
	struct SpotLight
	{
		Vector3f position = {};
		Vector3f direction = {0.f, -1.f, 0.f};
		Color color = { 1.f, 1.f, 1.f, 1.f };
		float range = 1000.0f;
		float innerConeAngleDegrees = 15.0f;
		float outerConeAngleDegrees = 25.0f;
		Matrix4x4f worldToLightClip = {};
	};
}