#include <graphics.h>
#include <sides.h>

#include <SDL2/SDL.h>

#pragma once

struct RectangleColor {
    Uint8 red;
    Uint8 green;
    Uint8 blue;
    Uint8 alpha;
};

class Rectangle {
public:
    Rectangle() {}

    Rectangle(int x, int y, int width, int height) :
        x(x),
        y(y),
        width(width),
        height(height) {
    }

    const int get_center_x() const {
        return x + width / 2;
    }

    const int get_center_y() const {
        return y + height / 2;
    }

    const int get_left() const{
        return x;
    }

    const int get_right() const {
        return x + width;
    }

    const int get_top() const {
        return y;
    }

    const int get_bottom() const {
        return y + height;
    }

    const int get_width() const {
        return width;
    }

    const int get_height() const {
        return height;
    }

    const int get_side(const sides::Side side) const {
        return
            side == sides::Left ? get_left() :
            side == sides::Right ? get_right() :
            side == sides::Top ? get_top() :
            side == sides::Bottom ? get_bottom() :
            -1;
    }

    // TODO: LimeOats passed in by reference, but for my own amusement,
    // I will try to pass in by pointer. Need to investigate what the implications
    // of this are.
    const bool collides_with(const Rectangle* rectangle) {
        return
            get_right() >= rectangle->get_left() &&
            get_left() <= rectangle->get_right() &&
            get_top() <= rectangle->get_bottom() &&
            get_bottom() >= rectangle->get_top();
    }

    // READ: const bool in a function declaration - what does this do?
    const bool is_valid_rectangle() {
        return
            x >= 0 &&
            y >= 0 &&
            width >= 0 &&
            height >= 0;
    }

    void draw(Graphics &graphics, const RectangleColor& color = { 0, 0, 0, 255 }) {
        SDL_Rect rect = { x, y, width, height };
        auto renderer = graphics.get_renderer();

        // Uint8 r, g, b, a;
        // SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

        SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, color.alpha);
        SDL_RenderDrawRect(renderer, &rect);
        // SDL_SetRenderDrawColor(renderer, r, g, b, a);
    }
private:
    int x, y, width, height;
};