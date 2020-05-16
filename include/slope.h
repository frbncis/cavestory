#include <rectangle.h>
#include <vector2.h>

#include <cstdlib>
#include <iostream>
#include <float.h>

#pragma once

class Slope {
public:
    Slope() { }
    Slope(Vector2 point_1, Vector2 point_2) {
        // Define the Slope object such that
        // the start_point is always to the left of end_point.

        // TODO: Slope calculations not working.
        if (point_1.x <= point_2.x) {
            start_point = point_1;
            end_point = point_2;
        } else {
            start_point = point_2;
            end_point = point_1;
        }

        if (point_1.x != point_2.x) {
            slope = ((float)point_2.y - (float)point_1.y) / ((float)point_2.x - (float)point_1.x);
        } else {
            slope = FLT_MAX;
        }

        if (slope == 0) {
            std::cout << "Got zero slope\n";
        }
    }

    // TODO: What is/why is this const inlined?
    const inline float get_slope() const {
        return slope;
    }

    const bool collides_with(const Rectangle* rectangle) {
        bool collides = false;

        // Negative slopes are those that go this way --> /
        if (slope < 0) {
            collides = 
                rectangle->get_center_x() >= start_point.x &&
                rectangle->get_center_x() <= end_point.x &&
                rectangle->get_bottom() <= start_point.y &&
                rectangle->get_bottom() >= end_point.y;
        } else {
            collides =
                rectangle->get_center_x() <= end_point.x &&
                rectangle->get_center_x() >= start_point.x &&
                rectangle->get_bottom() >= start_point.y &&
                rectangle->get_bottom() <= end_point.y;
        }

        return collides;
    }

    Vector2 get_start_point() {
        return start_point;
    }

    Vector2 get_endpoint() {
        return end_point;
    }

    int get_position_y(int position_x) {
        // y = mx + b
        // b = y - mx
        float intercept = (float)end_point.y - slope * (float)end_point.x;

        float position_y = slope * (float)position_x + (float)intercept;

        return (int)std::ceil(position_y);
    }

    void draw(Graphics &graphics, const RectangleColor& color = { 0, 0, 0, 255 }) {
        auto renderer = graphics.get_renderer();

        SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, color.alpha);
        SDL_RenderDrawLine(renderer, start_point.x, start_point.y, end_point.x, end_point.y);
    }

private:
    Vector2 start_point, end_point;

    // Returns the magnitude of the slope.
    //
    // The magnitude is defined such that the y-axis runs on the 
    // left edge of the screen while the x-axis runs along the top edge 
    // of the screen.
    float slope;
};