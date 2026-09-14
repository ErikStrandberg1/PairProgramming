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
		Vector3f direction = { 0.f, -1.f, 0.f };   // normalized, cone axis
		Color color = { 1.f, 1.f, 1.f, 1.f };
		float range = 1000.0f;
		float innerConeAngleDegrees = 15.0f;       // full brightness inside this half-angle
		float outerConeAngleDegrees = 25.0f;       // falls to 0 at this half-angle
		Matrix4x4f worldToLightClip = {};          // filled in once per frame, used to both render *and* sample the shadow map
	};
}