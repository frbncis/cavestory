#include <level.h>

#include <tinyxml2.h>

#include <string>

#pragma once

class Graphics;

using namespace tinyxml2;

class TsxLevelFactory {
public:
    TsxLevelFactory(Graphics &graphics);
    ~TsxLevelFactory();

    Level load_map(std::string map_name);

private:
    std::vector<TileSet> parse_tile_sets(XMLElement* map_node);
    std::vector<Tile> parse_tiles(XMLElement* map_node, std::vector<TileSet> tile_sets, Vector2 map_size, Vector2 tile_size);
    
    Graphics graphics;
};