#include <graphics.h>
#include <iostream>
#include <sprite.h>
#include <string>

Sprite::Sprite() {}

Sprite::Sprite(Graphics &graphics, 
    const std::string &file_path,
    int source_x,
    int source_y,
    int width,
    int height,
    float position_x,
    float position_y): 
        position_x(position_x),
        position_y(position_y) {

    // READ: Who initializes this? It was declared in the header file but 
    // but no one new'ed an object? FWIW, this is a struct, need to see
    // how structs get initialized.
    this->source_rect.x = source_x;
    this->source_rect.y = source_y;
    this->source_rect.w = width;
    this->source_rect.h = height;

    std::cout << "Loading sprite sheet at " << file_path << "\n";

    auto* renderer = graphics.get_renderer();
    this->sprite_sheet = SDL_CreateTextureFromSurface(renderer, graphics.load_image(file_path));

    // READ: I used nullptr, but he used null.. Are
    // they equivalent?
    if (this->sprite_sheet == NULL) {
        std::cout << "Unable to load image at " << file_path;
    }
}

Sprite::~Sprite() { }

void Sprite::update() { }

void Sprite::draw(Graphics &graphics, int x, int y) {
    SDL_Rect destination_rectangle = { x, y, this->source_rect.w, this->source_rect.h };

    graphics.blit_surface(this->sprite_sheet, &this->source_rect, &destination_rectangle);
}