#include <level.h>
#include <graphics.h>
#include <vector2.h>

#include <SDL2/SDL.h>

Level::Level() {}

Level::Level(std::string map_name, Vector2 spawn_point, Graphics &graphics):
    map_name(map_name),
    spawn_point(spawn_point),
    size(Vector2::zero()) {

    load_map(map_name, graphics);
}

Level::~Level() { }

void Level::load_map(std::string map_name, Graphics &graphics) {
    // TODO: Refactor
    background_texture = SDL_CreateTextureFromSurface(
        graphics.get_renderer(),
        graphics.load_image("content/backgrounds/bkBlue.png")
    );

    size = Vector2(graphics.screen_width * 2, graphics.screen_height * 2);
}

void Level::update(int time_elapsed) {

}

void Level::draw(Graphics &graphics) {
    SDL_Rect source_rectangle = { 0, 0, 64, 64 };
    SDL_Rect destination_rectangle;

    for (int x = 0; x < size.x / 64; x++) {
        for (int y = 0; y < size.y / 64; y++) {
            destination_rectangle.x = x * 64;
            destination_rectangle.y = y * 64;
            destination_rectangle.h = 64;
            destination_rectangle.w = 64;

            graphics.blit_surface(background_texture, &source_rectangle, &destination_rectangle);
        }
    }
}