#include <vector2.h>
#pragma once

struct SDL_Texture;
class Graphics;

class Tile {
public:
    Tile();
    Tile(SDL_Texture* tile_set, Vector2 size, Vector2 tile_set_position, Vector2 map_position);

    void update(int time_elapsed);

    void draw(Graphics &graphics);

protected:
    /**
     * Position of the tile on the map.
     */
    Vector2 map_position;

    Vector2 size;

    SDL_Texture* tile_set;

private:
    /**
     * Position of the tile on the tile set.
     */
    Vector2 tile_set_position;

};