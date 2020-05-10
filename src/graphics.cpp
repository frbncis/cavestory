#include <graphics.h>
#include <SDL2/SDL.h>
#include <string>

const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 640;
const char* SCREEN_TITLE = "Cave Story";

Graphics::Graphics() {
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &_window, &_renderer);
    SDL_SetWindowTitle(_window, SCREEN_TITLE);
}

Graphics::~Graphics() {
    SDL_DestroyWindow(_window);
}