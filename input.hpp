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

std::vector<bool> InputHandler::isPressed = std::vector<bool>(256, false);

bool InputHandler::IsPressed(char c)
{
	return isPressed[c];
}

bool InputHandler::IsUpPressed()
{
	return IsPressed(VK_UP);
}

bool InputHandler::IsDownPressed()
{
	return IsPressed(VK_DOWN);
}

bool InputHandler::IsLeftPressed()
{
	return IsPressed(VK_LEFT);
}

bool InputHandler::IsRightPressed()
{
	return IsPressed(VK_RIGHT);
}

void InputHandler::Update()
{
	for (size_t i = 0; i < isPressed.size(); i++)
	{
		isPressed.at(i) = false;
	}

	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD numberOfInputEvents;
	GetNumberOfConsoleInputEvents(hInput, &numberOfInputEvents);

	for (size_t i = 0; i < numberOfInputEvents; i++)
	{
		INPUT_RECORD inputRecord;
		DWORD events;
		ReadConsoleInput(hInput, &inputRecord, 1, &events);  // TODO: I think this is causing slowness.

		if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown)
		{
			int keyCode = inputRecord.Event.KeyEvent.wVirtualKeyCode;

			// 0x41 is the Windows virtual keycode for key A, 0x61 is the ASCII code for the character 'a'.
			if (keyCode > 0x41) keyCode = keyCode - 0x41 + 0x61;

			isPressed.at(keyCode) = true;
		}
	}
}
