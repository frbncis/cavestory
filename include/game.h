#include <animated_sprite.h>
#include <graphics.h>
#include <input.h>

#include <sprite.h>

#pragma once

class Game {
public:
    Game();
    ~Game();
    void start_game_loop();
private:
    AnimatedSprite player_sprite;
    Input input;
    
    void draw(Graphics &graphics);
    void update(float elapsed_time);
};