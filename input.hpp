#pragma once

#include <vector>

#define NOMINMAX
#include "windows.h"


class InputHandler
{
public:
	static bool IsPressed(char c);
	static bool IsUpPressed();
	static bool IsDownPressed();
	static bool IsLeftPressed();
	static bool IsRightPressed();
	static void Update();
private:

	static std::vector<bool> isPressed;
};
