#include <chrono>
#include <iostream>
#include <game.h>
#include <graphics.h>
#include <thread>

int main() {
	std::cout << "Hello World!\n";

	Game game;

	game.start_game_loop();

	return 0;
}
