#include <animated_sprite.h>
#include <graphics.h>
#include <input.h>
#include <level.h>
#include <player.h>

#pragma once

class Game {
public:
    static bool GAME_DEBUG;

    Game();
    ~Game();

    void start_game_loop();
private:
    Player player_sprite;
    Input input;
    Level level;
    
    void draw(Graphics &graphics);
    void update(float elapsed_time);
};