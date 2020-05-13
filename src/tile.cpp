#include<graphics.h>
#include<tile.h>

#include<SDL2/SDL.h>

Tile::Tile() {}

Tile::Tile(SDL_Texture* tile_set, Vector2 size, Vector2 tile_set_position, Vector2 map_position) :
    tile_set(tile_set),
    size(size),
    tile_set_position(tile_set_position),
    map_position(map_position) {

}

void Tile::update(int time_elapsed) { }

void Tile::draw(Graphics &graphics) {
    SDL_Rect destination_rectangle = { 
        map_position.x * graphics.sprite_scale_factor,
        map_position.y * graphics.sprite_scale_factor,
        size.x * graphics.sprite_scale_factor,
        size.y * graphics.sprite_scale_factor,
    };

    SDL_Rect source_rectangle = {
        tile_set_position.x,
        tile_set_position.y,
        size.x,
        size.y,
    };

    graphics.blit_surface(tile_set, &source_rectangle, &destination_rectangle);
}