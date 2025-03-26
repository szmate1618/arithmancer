#include <iostream>
#include <chrono>
#include <thread>

#define NOMINMAX
#include "windowing.hpp"
#include "map.hpp"
#include "screen.hpp"
#include "game.hpp"


const int TARGET_FPS = 30;
const int FRAME_TIME = 1000 / TARGET_FPS; // 33.33 ms per frame

int main() {

	//std::ios_base::sync_with_stdio(false);

	SetConsoleSize(100, 50);

	//Map map(100, 50, 20);

	ScreenBuffer screenBuffer(100, 50);
	//map.PrintDungeon(screenBuffer);
	screenBuffer.Display();
	//Menu menu;

	Game game;
	while (game.IsRunning()) {
		auto frame_start = std::chrono::steady_clock::now();

		game.Update(1.0 / TARGET_FPS);
		game.Draw(screenBuffer);
		SetCursorPosition(0, 0);
		screenBuffer.Display();

		/* InputHandler::Update();
		 menu.Update();
		 menu.Draw(screenBuffer);
		 SetCursorPosition(0, 0);
		 screenBuffer.Display();*/

		auto frame_end = std::chrono::steady_clock::now();
		std::chrono::duration<double, std::milli> frame_duration = frame_end - frame_start;

		if (frame_duration.count() < FRAME_TIME) {
			std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_TIME) - frame_duration);
		}
	}
	return 0;
}
