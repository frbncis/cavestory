#include <graphics.h>
#include <sprite.h>

#pragma once

class Game {
public:
    Game();
    ~Game();
    void start_game_loop();
private:
    Sprite player_sprite;
    
    void draw(Graphics &graphics);
    void update(float elapsed_time);
};