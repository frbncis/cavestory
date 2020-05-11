#include <graphics.h>

#pragma once

class Game {
public:
    Game();
    ~Game();
    void start_game_loop();
private:
    void draw(Graphics &graphics);
    void update(float elapsed_time);
};