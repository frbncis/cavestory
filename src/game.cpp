#include <game.h>
#include <getexepath.h>
#include <graphics.h>
#include <input.h>
#include <iostream>
#include <player.h>
#include <rectangle.h>
#include <SDL2/SDL.h>

#include <vector>

const int FPS = 50;
const int MAX_FRAME_TIME = 5 * 1000 / FPS;

bool Game::GAME_DEBUG;

Game::Game() {
    // TODO: Should this be moved to be handled by Graphics?
    SDL_Init(SDL_INIT_EVERYTHING);

    std::cout << "Game engine started in folder " << getexepath() << "\n";
}

Game::~Game() {

}

void Game::start_game_loop() {
    Graphics graphics;
    SDL_Event event;

    level = Level("Map 1", graphics);
    player_sprite = Player(graphics, level.get_player_spawn_point());

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

        if (input.was_key_pressed(SDL_SCANCODE_Z)) {
            player_sprite.jump();
        }

        if (input.was_key_pressed(SDL_SCANCODE_F3)) {
            Game::GAME_DEBUG = !Game::GAME_DEBUG;
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

    level.draw(graphics);
    player_sprite.draw(graphics);

    graphics.flip();
}

void Game::update(float time_since_last_game_frame_update)  {
    level.update(time_since_last_game_frame_update);
    player_sprite.update(time_since_last_game_frame_update);

    Rectangle player_bounding_box = player_sprite.get_bounding_box();

    std::vector<Rectangle> colliding_rectangles = level.get_colliding_rectangle(player_bounding_box);
    std::vector<Slope> colliding_slopes = level.get_colliding_slopes(player_bounding_box);

    if (colliding_rectangles.size() > 0) {
        player_sprite.on_tile_collision(colliding_rectangles);
    }

    player_sprite.on_slope_collision(colliding_slopes);
}