#include <animated_tile.h>
#include <tile.h>
#include <rectangle.h>
#include <slope.h>
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
    Level(std::string map_name, std::vector<Tile> tiles, std::vector<AnimatedTile> animated_tiles, std::vector<TileSet> tile_sets, std::vector<Rectangle> collidable_rectangles, std::vector<Slope> collidable_slopes);
    ~Level();

    void update(int time_elapsed);
    void draw(Graphics &graphics);

    std::vector<Rectangle> get_colliding_rectangle(Rectangle &rectangle);
    std::vector<Slope> get_colliding_slopes(Rectangle &rectangle);

    Vector2 get_player_spawn_point();
    void set_player_spawn_point(int x, int y);

private:
    std::string map_name;
    Vector2 spawn_point;

    std::vector<Tile> tiles;
    std::vector<TileSet> tile_sets;

    std::vector<AnimatedTile> animated_tiles;
    std::vector<AnimatedTileInfo> animated_tile_infos;

    std::vector<Rectangle> collidable_rectangles;
    std::vector<Slope> collidable_slopes;
};