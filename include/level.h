#include <string>
#include <vector2.h>

#pragma once

class Graphics;
struct SDL_Texture;

class Level {
public:
    Level();
    Level(std::string map_name, Vector2 spawn_point, Graphics &graphics);
    ~Level();

    void update(int time_elapsed);
    void draw(Graphics &graphics);

private:
    std::string map_name;
    Vector2 spawn_point;

    Vector2 size;

    SDL_Texture* background_texture;

    void load_map(std::string map_name, Graphics &graphics);
};