#include <game.h>
#include <graphics.h>
#include <input.h>
#include <iostream>
#include <player.h>
#include <SDL2/SDL.h>

const int FPS = 50;
const int MAX_FRAME_TIME = 5 * 1000 / FPS;

Game::Game() {
    // TODO: Should this be moved to be handled by Graphics?
    SDL_Init(SDL_INIT_EVERYTHING);
}

Game::~Game() {

}

void Game::start_game_loop() {
    Graphics graphics;
    SDL_Event event;

    player_sprite = Player(graphics, 100, 100);

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
        } else if (input.is_key_held(SDL_SCANCODE_LEFT)) {
            player_sprite.move_left();
        } else if (input.is_key_held(SDL_SCANCODE_RIGHT)) {
            player_sprite.move_right();
        } else {
            player_sprite.stop_moving();
        }

        if (!input.is_key_held(SDL_SCANCODE_LEFT) && !input.is_key_held(SDL_SCANCODE_RIGHT)) {
            player_sprite.stop_moving();
        }

        const int current_time_ms = SDL_GetTicks();
        int time_since_last_game_frame_update = current_time_ms - last_update_time;
        
        this->update(std::min(time_since_last_game_frame_update, MAX_FRAME_TIME));
        
        last_update_time = current_time_ms;

        this->draw(graphics);
    }
}

void Game::draw(Graphics &graphics) {
    graphics.clear();

    player_sprite.draw(graphics);

    graphics.flip();
}

void Game::update(float time_since_last_game_frame_update)  {
    player_sprite.update(time_since_last_game_frame_update);
}