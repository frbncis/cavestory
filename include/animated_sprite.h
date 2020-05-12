// READ: How to organize includes
#include<map>
#include<string>
#include<vector>

#include <sprite.h>
#include <vector2.h>

#pragma once

// READ: Type forwarding best practices.
class Graphics;

/**
 * Holds logic for animating sprites.
 */
class AnimatedSprite : public Sprite {
public:
    AnimatedSprite();

    AnimatedSprite(
        Graphics &graphics,
        const std::string &file_path,
        int source_x,
        int source_y,
        int width,
        int height,
        float position_x,
        float position_y,
        float sprite_frame_period);

    /**
     * Ensure that the animation is playing.
     */
    void play_animation(std::string animation, bool once = false);

    void update(int time_elapsed);

    void draw(Graphics &graphics, int x, int y);

    virtual void setup_animations();

protected:
    double sprite_frame_period;
    bool current_animation_once;
    std::string current_animation;

    void add_animation(int frame, int x, int y, std::string name, int width, int height, Vector2 offset);

    void reset_animations();

    void stop_animation();

    void set_visible(bool visible);

    virtual void on_animation_completed(std::string current_animation);

private:
    std::map<std::string, std::vector<SDL_Rect>> animations;
    std::map<std::string, Vector2> offsets;

    int frame_index;
    double time_since_sprite_frame_changed;
    bool visible;
};