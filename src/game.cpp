#include <game.h>
#include <graphics.h>
#include <input.h>
#include <iostream>
#include <SDL2/SDL.h>

const int FPS = 50;
const int MAX_FRAME_TIME = 5 * 1000 / FPS;

Game::Game() {
    // TODO: Should this be moved inside of Graphics?
    SDL_Init(SDL_INIT_EVERYTHING);
}

Game::~Game() {

}

void Game::start_game_loop() {
    Graphics graphics;
    Input input;
    SDL_Event event;

    int last_update_time = SDL_GetTicks();

    while (true) {
        input.begin_new_frame();

        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.repeat == 0) {
                    input.key_down_handler(event);
                }
            } else if (event.type == SDL_KEYUP) {
                input.key_up_handler(event);
            } else if (event.type == SDL_QUIT) {
                return;
            }
        }

        if (input.was_key_pressed(SDL_SCANCODE_ESCAPE)) {
            return;
        }

        // TODO: Why const?
        const int current_time_ms = SDL_GetTicks();
        int frame_time_ms = current_time_ms - last_update_time;
        
        this->update(std::min(frame_time_ms, MAX_FRAME_TIME));
        
        last_update_time = current_time_ms;
    }
}

void Game::draw(Graphics &graphics) {

}

void Game::update(float elapsed_time) {

}