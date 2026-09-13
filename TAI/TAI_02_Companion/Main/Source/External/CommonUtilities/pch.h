// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here

#include "framework.h"
#include "Timer.h"
#include "UtilityFunctions.h"
#include "InputHandler.h"
#include "Random.h"

//#include "Vector2.h"
//#include "Vector3.h"
//#include "Vector4.h"
//#include "Line.h"
//#include "Plane.h"

// DON'T include template classes here!
// Let each file include what it needs

#endif //PCH_H

// Only include pch.h at the top of .cpp files in the same library/project.

// How to enable/disable:
// Right-click your project (e.g., CommonUtilities) -> Properties.
// Configuration Properties -> C/C++ -> Precompiled Headers
// Precompiled Header: Use (/Yu)
// Precompiled Header File: pch.h

// You need one .cpp file (normally named pch.cpp) that creates the precompiled header.
// In its properties: Set Precompiled Header: Create (/Yc)
