#include "input.hpp"


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

	/*isPressed.at('s') = true;
	return;*/

	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

	DWORD numberOfInputEvents;
	GetNumberOfConsoleInputEvents(hInput, &numberOfInputEvents);
	if (numberOfInputEvents <= 0) return;

	INPUT_RECORD inputRecord[128];
	DWORD events;
	ReadConsoleInput(hInput, inputRecord, 128, &events);  // TODO: I think this is causing slowness.

	for (size_t i = 0; i < events; i++)
	{

		if (inputRecord[i].EventType == KEY_EVENT && inputRecord[i].Event.KeyEvent.bKeyDown)
		{
			int keyCode = inputRecord[i].Event.KeyEvent.wVirtualKeyCode;

			// 0x30 is the Windows virtual keycode for number key 0.
			if (keyCode > 0x30) keyCode = inputRecord[i].Event.KeyEvent.uChar.AsciiChar;

			isPressed.at(keyCode) = true;
		}
	}
}
