#include <tile.h>

#include <vector>

#pragma once

class AnimatedTile : public Tile {
public:
    AnimatedTile(std::vector<Vector2> frame_tile_positions, int frame_period, SDL_Texture* tile_set, Vector2 tile_size, Vector2 map_position);

    void update(int time_elapsed);

    void draw(Graphics &graphics);

protected:
    // WTF is this?
    int amount_of_time = 0;
    bool animation_complete = false;

private:
    std::vector<Vector2> frame_tile_positions;
    int current_frame;
    int frame_period;
};

struct AnimatedTileInfo {
public:
    int tile_set_first_gid;
    int start_tile_id;
    std::vector<int> tile_ids;
    int period;
};