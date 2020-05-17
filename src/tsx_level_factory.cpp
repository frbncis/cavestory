#include <graphics.h>
#include <string_split.h>
#include <tsx_level_factory.h>

#include <iostream>
#include <sstream>

#include <tinyxml2.h>

using namespace tinyxml2;

TsxLevelFactory::TsxLevelFactory(Graphics &graphics):
    graphics(graphics) {}

TsxLevelFactory::~TsxLevelFactory() {}

Level TsxLevelFactory::load_map(std::string map_name) {
    // TODO: Refactor this out to a tsx parser class.
    // Parse the tsx file to build out the map.
    // XML node traversal is pretty identical, would be a good place to practice
    // lambdas with recursion.

    // TODO: This group of variables is taken from Level's privates,
    // need to figure out what are relevant fields for Level.
    Vector2 spawn_point;
    Vector2 map_size;
    Vector2 tile_size;
    SDL_Texture* background_texture;
    std::vector<AnimatedTile> animated_tiles;
    std::vector<Tile> tiles;
    std::vector<TileSet> tile_sets;
    std::vector<Rectangle> collidable_rectangles;
    std::vector<Slope> collidable_slopes;

    std::vector<AnimatedTileInfo> animated_tile_infos;

    XMLDocument document;

    std::stringstream stream;
    stream << "maps/" << map_name << ".tmx";

    std::cout << "Loading map \"" << map_name << "\" at " << stream.str() << "\n";

    document.LoadFile(stream.str().c_str());

    XMLElement* map_node = document.FirstChildElement();

    // TODO: Does this get mad/modify a static version of
    // Vector2::zero()?
    map_node->QueryIntAttribute("width", &map_size.x);
    map_node->QueryIntAttribute("height", &map_size.y);

    map_node->QueryIntAttribute("tilewidth", &tile_size.x);
    map_node->QueryIntAttribute("tileheight", &tile_size.y);

    std::cout << "Map is " << map_size.x << " x " << map_size.y << "\n";

    tile_sets = parse_tile_sets(map_node, &animated_tile_infos);
    parse_tiles(map_node, tile_sets, map_size, tile_size, animated_tile_infos, &tiles, &animated_tiles);

    XMLElement* object_group_node = map_node->FirstChildElement("objectgroup");

    if (object_group_node != nullptr) {
        while (object_group_node) {
            const char* name = object_group_node->Attribute("name");

            std::stringstream string_stream;
            string_stream << name;

            if (string_stream.str() == "collisions") {
                std::cout << "Parsing collisions" << "\n";

                XMLElement* object_node = object_group_node->FirstChildElement("object");

                if (object_node != nullptr) {
                    while (object_node) {
                        float x, y, width, height;

                        int object_id = object_node->IntAttribute("id");

                        std::cout << "Processing collision object ID " << object_id << "\n";
                        
                        x = object_node->FloatAttribute("x");
                        y = object_node->FloatAttribute("y");
                        width = object_node->FloatAttribute("width");
                        height = object_node->FloatAttribute("height");

                        // TODO: Limeoats has his rectangle multipled by sprite scale.
                        // I don't like having what feels like a rendering property here,
                        // so make sure that it's handled elsewhere accoridngly.
                        collidable_rectangles.push_back(Rectangle(std::ceil(x) * 2, std::ceil(y) * 2, std::ceil(width) * 2, std::ceil(height) * 2));
                        
                        object_node = object_node->NextSiblingElement("object");
                    }
                }
            } else if (string_stream.str() == "spawn points") {
                std::cout << "Parsing spawn points" << "\n";

                XMLElement* object_node = object_group_node->FirstChildElement("object");

                if (object_node != nullptr) {
                    while (object_node) {
                        float x, y;
                        
                        x = object_node->FloatAttribute("x");
                        y = object_node->FloatAttribute("y");

                        const char* name = object_node->Attribute("name");

                        std::stringstream ss;

                        ss << name;

                        if (ss.str() == "player") {
                            //  TODO: Handle the sprite scale factor here.
                            spawn_point = Vector2(std::ceil(x) * 2, std::ceil(y) *2);
                        }

                        object_node = object_node->NextSiblingElement("object");
                    }
                }
            } else if (string_stream.str() == "slopes") {
                std::cout << "Parsing slopes" << "\n";

                XMLElement* object_node = object_group_node->FirstChildElement("object");

                if (object_node != nullptr) {
                    while (object_node) {
                        std::vector<Vector2> slope_points;
                        
                        Vector2 anchor = Vector2(
                            std::ceil(object_node->FloatAttribute("x")),
                            std::ceil(object_node->FloatAttribute("y"))
                        );

                        XMLElement* polyline = object_node->FirstChildElement("polyline");

                        if (polyline != nullptr) {
                            std::vector<std::string> line_segment_string_pair;

                            // TODO: Add extension method to extract strings from 
                            // XMLElement
                            const char* point_string = polyline->Attribute("points");

                            std::stringstream ss;

                            ss << point_string;

                            // TODO: Optimization possible, we can just scan along the string
                            // in a single pass and create string/int tokens based on ' ' and ','.
                            // Right now this takes a few passes through. *shrug*
                            line_segment_string_pair = split(ss.str(), ' ');

                            for (int i = 0; i < line_segment_string_pair.size() - 1; i++) {

                                auto start_points = split(line_segment_string_pair.at(i), ',');
                                auto end_points = split(line_segment_string_pair.at(i + 1), ',');

                                // TODO: Sprite scale
                                auto start_point_v = Vector2(
                                    anchor.x + std::stoi(start_points[0]),
                                    anchor.y + std::stoi(start_points[1])
                                ) * 2;

                                auto end_point_v = Vector2(
                                    anchor.x + std::stoi(end_points[0]),
                                    anchor.y + std::stoi(end_points[1])
                                ) * 2;

                                collidable_slopes.push_back(Slope(start_point_v, end_point_v));
                            }
                        }

                        object_node = object_node->NextSiblingElement("object");
                    }
                }
            }

            object_group_node = object_group_node->NextSiblingElement("objectgroup");
        }
    }

    std::cout << "Done loading map \"" << map_name << "\"!\n";

    Level level = Level(map_name, tiles, animated_tiles, tile_sets, collidable_rectangles, collidable_slopes);

    level.set_player_spawn_point(spawn_point.x, spawn_point.y);

    return level;
}

std::vector<TileSet> TsxLevelFactory::parse_tile_sets(XMLElement* map_node, std::vector<AnimatedTileInfo>* animated_tile_infos) {
    std::vector<TileSet> tile_sets;

    // Load the tile sets.
    XMLElement* tile_set_node = map_node->FirstChildElement("tileset");

    if (tile_set_node != nullptr) {
        while (tile_set_node) {
            int first_gid;

            const char* tile_set_path = tile_set_node->FirstChildElement("image")->Attribute("source");

            std::stringstream resolved_tile_set_path_builder;

            // The map files reference image assets relative to themselves
            // in the map folder, but we are executing in the project root.
            resolved_tile_set_path_builder << "maps/" << tile_set_path;

            std::cout << "Loading tile set at " << resolved_tile_set_path_builder.str() << "\n";

            tile_set_node->QueryIntAttribute("firstgid", &first_gid);

            SDL_Texture* tile_set_texture = SDL_CreateTextureFromSurface(
                graphics.get_renderer(),
                graphics.load_image(resolved_tile_set_path_builder.str()));

            if (tile_set_texture == nullptr) {
                std::cout << "Failed to load tile set texture for " << resolved_tile_set_path_builder.str() << "\n";
            }
            tile_sets.push_back(TileSet(tile_set_texture, first_gid));

            parse_animated_tile_infos(tile_set_node, first_gid, animated_tile_infos);

            tile_set_node = tile_set_node->NextSiblingElement("tileset");
        }
    }

    return tile_sets;
}

void TsxLevelFactory::parse_animated_tile_infos(XMLElement* tile_set_node, int tile_set_first_gid, std::vector<AnimatedTileInfo>* animated_tile_infos) {
    XMLElement* animated_tile_node = tile_set_node->FirstChildElement("tile");

    if (animated_tile_node != nullptr) {
        while (animated_tile_node) {

            auto tile_id_c = animated_tile_node->Attribute("id");

            std::stringstream ss;
            ss << tile_id_c;
            std::cout << "Parsing tile node ID=" << ss.str() << "\n";
            
            AnimatedTileInfo animated_tile_info;

            // TODO: Should the handling the offset for tile_set_first_gid be handled inside the tile info object?
            animated_tile_info.start_tile_id = animated_tile_node->IntAttribute("id") + tile_set_first_gid;
            animated_tile_info.tile_set_first_gid = tile_set_first_gid;

            XMLElement* animation_node = animated_tile_node->FirstChildElement("animation");

            if (animation_node != nullptr) {
                while (animation_node) {
                    std::cout << "Processing animation..." << "\n";

                    XMLElement* frame_node = animation_node->FirstChildElement("frame");

                    if (frame_node != nullptr) {
                        while (frame_node) {
                            auto frame_tile_id = frame_node->IntAttribute("tileid");
                            std::cout << "Parsing frame with tileid=" << frame_tile_id << "\n";

                            animated_tile_info.tile_ids.push_back(frame_tile_id + tile_set_first_gid);
                            animated_tile_info.period = frame_node->IntAttribute("duration");

                            frame_node = frame_node->NextSiblingElement("frame");
                        }
                    }

                    animation_node = animation_node->NextSiblingElement("animation");
                }
            }

            animated_tile_infos->push_back(animated_tile_info);

            animated_tile_node = animated_tile_node->NextSiblingElement("tile");
        }
    }
}

void TsxLevelFactory::parse_tiles(
    XMLElement* map_node,
    std::vector<TileSet> tile_sets,
    Vector2 map_size,
    Vector2 tile_size,
    std::vector<AnimatedTileInfo> animated_tile_infos,
    std::vector<Tile>* tiles,
    std::vector<AnimatedTile>* animated_tiles) {
    XMLElement* layer_element = map_node->FirstChildElement("layer");

    // TODO: Refactor this to a TmxParser class.
    if (layer_element != nullptr) {
        while (layer_element) {
            XMLElement* data_element = layer_element->FirstChildElement("data");

            // READ: How does char* work? How does strinstream know when it ends?
            // I was expecting char[]*.
            int data_id = data_element->IntAttribute("id");

            std::cout << "Processing tmx layer element ID=" << data_element << "\n";

            if (data_element != nullptr) {
                while (data_element) {

                    XMLElement* tile_element = data_element->FirstChildElement("tile");

                    if (tile_element != nullptr) {
                        int tile_counter = 0;

                        while (tile_element) {
                            std::cout << "Processing tile number " << tile_counter << "\n";

                            int current_gid = tile_element->IntAttribute("gid");

                            if (current_gid == 0) {
                                tile_counter++;
                                
                                if (tile_element->NextSiblingElement("tile")) {
                                    tile_element = tile_element->NextSiblingElement("tile");
                                    continue;
                                } else {
                                    break;
                                }
                            }

                            int gid = tile_element->IntAttribute("gid");
                            TileSet tile_set;
                            int closest = 0;

                            // TODO: Range operator?
                            for (int i = 0; i < tile_sets.size(); i++) {
                                if (tile_sets[i].first_gid <= gid) {
                                    if (tile_sets.at(i).first_gid > closest) {
                                        closest = tile_sets.at(i).first_gid;
                                        tile_set = tile_sets[i];
                                    }
                                }
                            }

                            if (tile_set.first_gid == -1) {
                                tile_counter++;

                                if (tile_element->NextSiblingElement("tile")) {
                                    tile_element = tile_element->NextSiblingElement("tile");
                                    continue;
                                }
                                else {
                                    break;
                                }
                            }

                            // Convert the tile_counter to x, y coordinates
                            // on the map.
                            int xx = (tile_counter % map_size.x) * tile_size.x;
                            int yy = (tile_counter / map_size.x) * tile_size.y;
                            Vector2 tile_position = Vector2(xx, yy);

                            Vector2 tile_set_position = get_tile_set_position(tile_set, gid, tile_size);

                            AnimatedTileInfo* animated_tile_info = nullptr;

                            for (int i = 0; i < animated_tile_infos.size(); i++) {
                                if (animated_tile_infos.at(i).start_tile_id == gid) {
                                    animated_tile_info = &animated_tile_infos.at(i);
                                    break;
                                }
                            }

                            if (animated_tile_info != nullptr) {
                                std::vector<Vector2> tile_set_positions;

                                for (int i = 0; i < animated_tile_info->tile_ids.size(); i++) {
                                    tile_set_positions.push_back(
                                        get_tile_set_position(
                                            tile_set,
                                            animated_tile_info->tile_ids.at(i),
                                            tile_size));
                                }

                                AnimatedTile tile(
                                    tile_set_positions,
                                    animated_tile_info->period,
                                    tile_set.texture,
                                    tile_size,
                                    tile_position);

                                std::cout << "AnimatedTile " << tile_counter << " at position (" << tile_position.x << "," << tile_position.y << ")\n";

                                animated_tiles->push_back(tile);

                            } else {
                                std::cout << "Tile " << tile_counter << " at position (" << tile_position.x << "," << tile_position.y << ")\n";

                                // Build Tile object and add it to level tile list.
                                Tile tile(tile_set.texture, tile_size, tile_set_position, tile_position);

                                tiles->push_back(tile);
                            }

                            tile_counter++;
                            tile_element = tile_element->NextSiblingElement("tile");
                        }
                    }

                    data_element = data_element->NextSiblingElement("data");
                }
            }

            layer_element = layer_element->NextSiblingElement("layer");
        }
    }
}


Vector2 TsxLevelFactory::get_tile_set_position(TileSet tile_set, int gid, Vector2 tile_size) {
    // Get the tile's position in the tile set.
    int tsxx = gid % (tile_set.width / tile_size.x) - 1;
    tsxx *= tile_size.x;

    int amount = ((gid - tile_set.first_gid) / (tile_set.width / tile_size.x));

    int tsyy = 0;
    tsyy = tile_size.y * amount;

    Vector2 tile_set_position = Vector2(tsxx, tsyy);

    return tile_set_position;
}