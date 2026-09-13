#include "pch.h"
#include "InputTest.h"
#include <iostream>
#include "Print.h"

namespace InputTest
{
	input* myInputHandler = nullptr;
	HWND myMainWindowHandle = nullptr;
	bool myMouseLocked = false;

	POINT GetClientCenterScreenSpace(HWND aHwnd)
	{
		RECT rc{};
		GetClientRect(aHwnd, &rc);
		POINT tl{rc.left, rc.top};
		POINT br{rc.right, rc.bottom};
		ClientToScreen(aHwnd, &tl);
		ClientToScreen(aHwnd, &br);
		POINT center{
			tl.x + (br.x - tl.x) / 2,
			tl.y + (br.y - tl.y) / 2
		};
		return center;
	}

	//

	void PrintInstructions()
	{
		std::cout <<
			"=== Input Test Controls ===\n"
			"[Tangent]  Any key: KeyPressed-test\n"
			"[Tangent]  Any key: KeyDown/held-test\n"
			"[Tangent]  Any key: KeyReleased-test\n"
			"[Mus]      L/R/M-button: Down/Pressed/Released\n"
			"[Mus]      Move mouse: pos/delta\n"
			"[Hjul]     Scroll: wheel-delta\n"
			"[Extra]    M: set mouse pos till mitten\n"
			"[Extra]    C: capture/lock mouse\n"
			"[Extra]    U: unlock mouse\n"
			"[Exit]     ESC: Quit\n"
			"===========================\n\n";
	}

	void RunTests()
	{
		auto& inputRef = *myInputHandler;

		// === Buttons ===
		if (inputRef.IsKeyPressed(KEY_SPACE))
		{
			std::cout << "[KeyPressed] SPACE\n";
		}

		if (inputRef.IsKeyDown(KEY_SHIFT))
		{
			std::cout << "[KeyDown] SHIFT\n";
		}

		if (inputRef.IsKeyReleased(KEY_CTRL))
		{
			std::cout << "[KeyReleased] CTRL\n";
		}

		if (globalInputHandler.IsKeyPressed(globalInputHandler.GetLastKeyPressed()))
		{
			std::cout << "[KeyPressed] " << static_cast<char>(globalInputHandler.GetLastKeyPressed()) << "\n";
		}

		if (globalInputHandler.IsKeyDown(globalInputHandler.GetLastKeyPressed()))
		{
			std::cout << "[KeyDown] " << static_cast<char>(globalInputHandler.GetLastKeyPressed()) << "\n";
		}

		if (globalInputHandler.IsKeyReleased(globalInputHandler.GetLastKeyReleased()))
		{
			std::cout << "[KeyReleased] " << static_cast<char>(globalInputHandler.GetLastKeyReleased()) << "\n";
		}

		// === Mouse Buttons ===
		if (inputRef.IsKeyPressed(VK_LBUTTON)) std::cout << "[Pressed] LMB\n";
		if (inputRef.IsKeyPressed(VK_RBUTTON)) std::cout << "[Pressed] RMB\n";
		if (inputRef.IsKeyPressed(VK_MBUTTON)) std::cout << "[Pressed] MMB\n";

		if (inputRef.IsKeyReleased(VK_LBUTTON)) std::cout << "[Released] LMB\n";
		if (inputRef.IsKeyReleased(VK_RBUTTON)) std::cout << "[Released] RMB\n";
		if (inputRef.IsKeyReleased(VK_MBUTTON)) std::cout << "[Released] MMB\n";

		if (inputRef.IsKeyDown(VK_LBUTTON)) std::cout << "[Down] LMB\n";
		if (inputRef.IsKeyDown(VK_RBUTTON)) std::cout << "[Down] RMB\n";
		if (inputRef.IsKeyDown(VK_MBUTTON)) std::cout << "[Down] MMB\n";

		// === Mouse ===
		if (inputRef.GetMouseDelta().x != 0 || inputRef.GetMouseDelta().y != 0)
		{
			std::cout << "[MousePos] ";
			PrintInput::PrintMousePOS();
			std::cout << " [Delta] ";
			PrintInput::PrintMouseDelta();
			std::cout << "\n";
		}

		// === Wheel ===
		if (inputRef.GetWheelDelta() != 0)
			std::cout << "[Wheel] delta=" << inputRef.GetWheelDelta() << "\n";

		// === Extra ===
		if (inputRef.IsKeyPressed('M'))
		{
			POINT center = GetClientCenterScreenSpace(myMainWindowHandle);
			inputRef.SetMousePosition(center);
			std::cout << "[SetMousePosition] center (" << center.x << "," << center.y << ")\n";
		}

		if (inputRef.IsKeyPressed('C'))
		{
			if (!myMouseLocked)
			{
				inputRef.LockMouseToWindow(myMainWindowHandle);
				myMouseLocked = true;
				std::cout << "[Capture] locked to window\n";
			}
			else
			{
				inputRef.UnlockFromWindow();
				myMouseLocked = false;
				std::cout << "[Capture] unlocked\n";
			}
		}

		if (inputRef.IsKeyPressed('U'))
		{
			inputRef.UnlockFromWindow();
			myMouseLocked = false;
			std::cout << "[Capture] unlocked (U)\n";
		}

		if (inputRef.IsKeyPressed(VK_ESCAPE))
		{
			PostQuitMessage(0);
		}
	}
}
