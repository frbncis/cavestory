#include <graphics.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const char* SCREEN_TITLE = "Cave Story";

Graphics::Graphics() {
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
    SDL_SetWindowTitle(window, SCREEN_TITLE);
}

Graphics::~Graphics() {
    SDL_DestroyWindow(window);
}

SDL_Surface* Graphics::load_image(const std::string &file_path) {
    if (this->sprite_sheets.count(file_path) == 0) {
        this->sprite_sheets[file_path] = IMG_Load(file_path.c_str());
    }

    return this->sprite_sheets[file_path];
}

void Graphics::blit_surface(SDL_Texture* texture, SDL_Rect* source_rectangle, SDL_Rect* destination_rectangle) {
    SDL_RenderCopy(this->renderer, texture, source_rectangle, destination_rectangle);
}

void Graphics::flip() {
    SDL_RenderPresent(this->renderer);
}

void Graphics::clear() {
    SDL_RenderClear(this->renderer);
}

SDL_Renderer* Graphics::get_renderer() {
    return this->renderer;
}