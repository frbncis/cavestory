#include <animated_sprite.h>
#include <graphics.h>
#include <input.h>
#include <player.h>

#pragma once

class Game {
public:
    Game();
    ~Game();
    void start_game_loop();
private:
    Player player_sprite;
    Input input;
    
    void draw(Graphics &graphics);
    void update(float elapsed_time);
};