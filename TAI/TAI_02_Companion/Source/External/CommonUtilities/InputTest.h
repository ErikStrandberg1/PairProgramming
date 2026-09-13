#pragma once
#include "InputHandler.h"
#include <Windows.h>

namespace InputTest
{
	using input = CommonUtilities::InputHandler;

	// global referenser (delas med main)
	extern input* myInputHandler;
	extern HWND myMainWindowHandle;
	extern bool myMouseLocked;

	void PrintInstructions();
	void RunTests();
}
