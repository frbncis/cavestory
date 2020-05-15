#include <tile.h>
#include <rectangle.h>
#include <vector2.h>

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include <SDL2/SDL.h>

#pragma once

class Graphics;

struct TileSet {
    SDL_Texture* texture;
    int first_gid;

    int width;
    int height;
    
    TileSet(): first_gid(-1) { }

    TileSet(SDL_Texture* texture, int first_gid):
        // TODO: Investigate if initializing this way
        // is equivalent to this->texture = texture
        // in the ctor body.
        texture(texture),
        first_gid(first_gid) {

        SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    }
};

class Level {
public:
    Level();
    Level(std::string map_name, Graphics &graphics);
    ~Level();

    void update(int time_elapsed);
    void draw(Graphics &graphics);

    std::vector<Rectangle> get_colliding_rectangle(Rectangle &rectangle);
    Vector2 get_player_spawn_point();

private:
    std::string map_name;
    Vector2 spawn_point;

    Vector2 size;
    Vector2 tile_size;

    SDL_Texture* background_texture;
    std::vector<Tile> tiles;
    std::vector<TileSet> tile_sets;

    std::vector<Rectangle> collidable_rectangles;

    void load_map(std::string map_name, Graphics &graphics);
};