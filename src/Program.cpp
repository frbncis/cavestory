#include <chrono>
#include <iostream>
#include <Graphics.h>
#include <thread>

int main() {
	std::cout << "Hello World!\n";

	Graphics graphics;

	std::this_thread::sleep_for(std::chrono::seconds(10));
	return 0;
}
