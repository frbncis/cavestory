#include <graphics.h>

#pragma once

class Sprite {
public:
    Sprite();
    Sprite(Graphics &graphics, const std::string &file_path, int source_x, int source_y, int width, int height, float position_x, float position_y);

    virtual ~Sprite();
    virtual void update();
    void draw(Graphics &graphics, int x, int y);

protected:
    SDL_Rect source_rect;
    SDL_Texture* sprite_sheet;

private:
    float position_x;
    float position_y;
};