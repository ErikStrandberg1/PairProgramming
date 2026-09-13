#include "pch.h"
#include "InputHandler.h"

#include <iostream>

using Input = CommonUtilities::InputHandler;

namespace PrintInput
{
	//<----------- Buttons ----------->
	void PrintLastKeyPressed()
	{
		if (globalInputHandler.IsKeyPressed(globalInputHandler.GetLastKeyPressed()))
		{
			std::cout << static_cast<char>(globalInputHandler.GetLastKeyPressed());
		}
	}

	void PrintLastKeyReleased()
	{
		if (globalInputHandler.IsKeyReleased(globalInputHandler.GetLastKeyReleased()))
		{
			std::cout << static_cast<char>(globalInputHandler.GetLastKeyReleased());
		}
	}

	void PrintLastKeyDown()
	{
		if (globalInputHandler.IsKeyDown(globalInputHandler.GetLastKeyPressed()))
		{
			std::cout << static_cast<char>(globalInputHandler.GetLastKeyPressed());
		}
	}

	//<----------- Mouse Movement ----------->
	void PrintMousePOS()
	{
		if (globalInputHandler.GetMouseDelta().x != 0 || globalInputHandler.GetMouseDelta().y != 0)
		{
			std::cout << "{" << globalInputHandler.GetMousePosition().x << "x,"
				<< globalInputHandler.GetMousePosition().y << "y}";
		}
	}

	void PrintMouseDelta()
	{
		std::cout << "{" << globalInputHandler.GetMouseDelta().x << "x,"
			<< globalInputHandler.GetMouseDelta().y << "y}";
	}

	//<----------- Mouse Buttons ----------->
	void PrintLMouseButtonDown()
	{
		std::cout << IsLButtonDown();
	}

	void PrintRMouseButtonDown()
	{
		std::cout << IsRButtonDown();
	}

	void PrintMMouseButtonDown()
	{
		std::cout << IsMButtonDown();
	}

	void PrintLMouseButtonPressed()
	{
		std::cout << globalInputHandler.IsKeyPressed(VK_LBUTTON);
	}

	void PrintRMouseButtonPressed()
	{
		std::cout << globalInputHandler.IsKeyPressed(VK_RBUTTON);
	}

	void PrintMMouseButtonPressed()
	{
		std::cout << globalInputHandler.IsKeyPressed(VK_MBUTTON);
	}

	void PrintLMouseButtonReleased()
	{
		std::cout << globalInputHandler.IsKeyReleased(VK_LBUTTON);
	}

	void PrintRMouseButtonReleased()
	{
		std::cout << globalInputHandler.IsKeyReleased(VK_RBUTTON);
	}

	void PrintMMouseButtonReleased()
	{
		std::cout << globalInputHandler.IsKeyReleased(VK_MBUTTON);
	}

	void PrintWheelDelta()
	{
		std::cout << globalInputHandler.GetWheelDelta();
	}

	void PrintMouseAbsolutePosition()
	{
		std::cout << "{" << globalInputHandler.GetMouseAbsolutePOS().x << "x,"
			<< globalInputHandler.GetMouseAbsolutePOS().y << "y}";
	}
}
