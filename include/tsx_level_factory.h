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
    std::vector<TileSet> parse_tile_sets(XMLElement* map_node, std::vector<AnimatedTileInfo>* animated_tile_infos);
    void parse_animated_tile_infos(XMLElement* tile_set_node, int tile_set_first_gid, std::vector<AnimatedTileInfo>* animated_tile_infos);

    void parse_tiles(XMLElement* map_node, std::vector<TileSet> tile_sets, Vector2 map_size, Vector2 tile_size, std::vector<AnimatedTileInfo> animated_tile_infos, std::vector<Tile>* tiles, std::vector<AnimatedTile>* animated_tiles);
    
    Vector2 get_tile_set_position(TileSet tile_set, int gid, Vector2 tile_size);

    Graphics graphics;
};