#include <animated_tile.h>
#include <graphics.h>
#include <tile.h>

AnimatedTile::AnimatedTile(
    std::vector<Vector2> frame_tile_positions,
    int frame_period,
    SDL_Texture* tile_set,
    Vector2 tile_size,
    Vector2 map_position) :
        Tile(tile_set, tile_size, frame_tile_positions.at(0), map_position),
        frame_tile_positions(frame_tile_positions),
        frame_period(frame_period),
        current_frame(0) {

}

void AnimatedTile::update(int time_elapsed) {
    if (amount_of_time <= 0) {
        if (current_frame == frame_tile_positions.size() - 1) {
            current_frame = 0;
        } else {
            current_frame += 1;
        }

        amount_of_time = frame_period;
    } else {
        amount_of_time -= time_elapsed;
    }

    Tile::update(time_elapsed);
}

void AnimatedTile::draw(Graphics &graphics) {
    // TODO: Sprite scale
    SDL_Rect destination_rectangle = {
        map_position.x * graphics.sprite_scale_factor,
        map_position.y * graphics.sprite_scale_factor,
        size.x * graphics.sprite_scale_factor,
        size.y * graphics.sprite_scale_factor
    };

    SDL_Rect source_rectangle = {
        frame_tile_positions.at(current_frame).x,
        frame_tile_positions.at(current_frame).y,
        size.x,
        size.y
    };

    graphics.blit_surface(tile_set, &source_rectangle, &destination_rectangle);
}