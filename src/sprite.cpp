#include <graphics.h>
#include <iostream>
#include <sprite.h>
#include <string>

#include <algorithm> 

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

    if (sprite_sheet == nullptr) {
        std::cout << "Unable to load image at " << file_path << "\n";
    }

    // TODO: Sprite scale factor handling
    bounding_box = Rectangle(position_x, position_y, width * 2, height * 2);
}

Sprite::~Sprite() { }

void Sprite::update() {
    // TODO: Sprite scale factor handling
    bounding_box = Rectangle(position_x, position_y, source_rect.w * 2, source_rect.h * 2);
}

void Sprite::draw(Graphics &graphics, int x, int y) {
    SDL_Rect destination_rectangle = { 
        x,
        y,
        source_rect.w * graphics.sprite_scale_factor,
        source_rect.h * graphics.sprite_scale_factor
    };

    graphics.blit_surface(this->sprite_sheet, &this->source_rect, &destination_rectangle);
}

Rectangle Sprite::get_bounding_box() {
    return bounding_box;
}

const sides::Side Sprite::get_colliding_side(Rectangle &colliding_rectangle) const {
    int overlap_right, overlap_left, overlap_top, overlap_bottom;

    overlap_right = abs(bounding_box.get_right() - colliding_rectangle.get_left());
    overlap_left = abs(colliding_rectangle.get_right() - bounding_box.get_left());
    overlap_top = abs(colliding_rectangle.get_bottom() - bounding_box.get_top());
    overlap_bottom = abs(bounding_box.get_bottom() - colliding_rectangle.get_top());

    int overlaps[] = { overlap_right, overlap_left, overlap_top, overlap_bottom };

    int* min_overlap = std::min_element(overlaps, overlaps + 4);

    return
        *min_overlap == overlap_right ? sides::Right :
        *min_overlap == overlap_left ? sides::Left :
        *min_overlap == overlap_top ? sides::Top :
        *min_overlap == overlap_bottom ? sides::Bottom :
            sides::None;
}