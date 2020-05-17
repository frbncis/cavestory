#include <level.h>
#include <game.h>
#include <graphics.h>
#include <rectangle.h>
#include <vector2.h>

#include <SDL2/SDL.h>
#include <tinyxml2.h>

#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace tinyxml2;

Level::Level() {}

Level::Level(
    std::string map_name,
    std::vector<Tile> tiles,
    std::vector<AnimatedTile> animated_tiles,
    std::vector<TileSet> tile_sets,
    std::vector<Rectangle> collidable_rectangles,
    std::vector<Slope> collidable_slopes):
        map_name(map_name),
        tiles(tiles),
        animated_tiles(animated_tiles),
        tile_sets(tile_sets),
        collidable_rectangles(collidable_rectangles),
        collidable_slopes(collidable_slopes) {
}

Level::~Level() { }

void Level::update(int time_elapsed) {
    for (int i = 0; i < animated_tiles.size(); i++) {
        animated_tiles.at(i).update(time_elapsed);
    }
}

void Level::draw(Graphics &graphics) {
    for (int i = 0; i < tiles.size(); i++) {
        tiles.at(i).draw(graphics);
    }

    for (int i = 0; i < animated_tiles.size(); i++) {
        animated_tiles.at(i).draw(graphics);
    }

    if (Game::GAME_DEBUG) {
        for (int i = 0; i < collidable_rectangles.size(); i++) {
            collidable_rectangles.at(i).draw(graphics, { 0, 0, 255, 255 });
        }

        for (int i = 0; i < collidable_slopes.size(); i++) {
            collidable_slopes.at(i).draw(graphics, { 0, 0, 255, 255, });
        }
    }
}

std::vector<Rectangle> Level::get_colliding_rectangle(Rectangle &rectangle) {
    std::vector<Rectangle> colliding_rectangles;

    for (int i = 0; i < collidable_rectangles.size(); i++) {
        if (collidable_rectangles.at(i).collides_with(&rectangle)) {
            colliding_rectangles.push_back(collidable_rectangles.at(i));
        }
    }

    return colliding_rectangles;
}

std::vector<Slope> Level::get_colliding_slopes(Rectangle &rectangle) {
    std::vector<Slope> colliding_slopes;

    for (int i = 0; i < collidable_slopes.size(); i++) {
        if (collidable_slopes.at(i).collides_with(&rectangle)) {
            colliding_slopes.push_back(collidable_slopes.at(i));
        }
    }

    return colliding_slopes;
}

void Level::set_player_spawn_point(int x, int y) {
    spawn_point = Vector2(x, y);
}

Vector2 Level::get_player_spawn_point() {
    return spawn_point;
}