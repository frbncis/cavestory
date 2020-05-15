#include <level.h>
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

Level::Level(std::string map_name, Graphics &graphics):
    map_name(map_name),
    size(Vector2::zero()) {

    load_map(map_name, graphics);
}

Level::~Level() { }

void Level::load_map(std::string map_name, Graphics &graphics) {
    // TODO: Refactor this out to a tsx parser class.
    // Parse the tsx file to build out the map.
    XMLDocument document;

    std::stringstream stream;
    stream << "maps/" << map_name << ".tmx";

    std::cout << "Loading map \"" << map_name << "\" at " << stream.str() << "\n";

    document.LoadFile(stream.str().c_str());

    XMLElement* map_node = document.FirstChildElement();

    // TODO: Does this get mad/modify a static version of
    // Vector2::zero()?
    map_node->QueryIntAttribute("width", &size.x);
    map_node->QueryIntAttribute("height", &size.y);

    map_node->QueryIntAttribute("tilewidth", &tile_size.x);
    map_node->QueryIntAttribute("tileheight", &tile_size.y);

    std::cout << "Map is " << size.x << " x " << size.y << "\n";

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

            tile_set_node = tile_set_node->NextSiblingElement("tileset");
        }
    }

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

                            // TODO: Range operator?
                            for (int i = 0; i < tile_sets.size(); i++) {
                                if (tile_sets[i].first_gid <= gid) {
                                    tile_set = tile_sets[i];
                                    break;
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
                            int xx = (tile_counter % size.x) * tile_size.x;
                            int yy = (tile_counter / size.x) * tile_size.y;
                            Vector2 tile_position = Vector2(xx, yy);

                            // Get the tile's position in the tile set.
                            // TODO: Refactor.
                            int tsxx = gid % (tile_set.width / tile_size.x) - 1;
                            tsxx *= tile_size.x;

                            int amount = (gid / (tile_set.width / tile_size.x));

                            int tsyy = 0;
                            tsyy = tile_size.y * amount;

                            Vector2 tile_set_position = Vector2(tsxx, tsyy);

                            std::cout << "Tile " << tile_counter << " at position (" << tile_position.x << "," << tile_position.y << ")\n";

                            // Build Tile object and add it to level tile list.
                            Tile tile(tile_set.texture, tile_size, tile_set_position, tile_position);

                            tiles.push_back(tile);

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
            }

            object_group_node = object_group_node->NextSiblingElement("objectgroup");
        }
    }

    std::cout << "Done loading map \"" << map_name << "\"!\n";
}

void Level::update(int time_elapsed) {

}

void Level::draw(Graphics &graphics) {
    for (int i = 0; i < tiles.size(); i++) {
        tiles.at(i).draw(graphics);
    }

    for (int i = 0; i < collidable_rectangles.size(); i++) {
        collidable_rectangles.at(i).draw(graphics);
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

Vector2 Level::get_player_spawn_point() {
    return spawn_point;
}