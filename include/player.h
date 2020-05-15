#include<animated_sprite.h>
#include<direction.h>

#pragma once

class Player : public AnimatedSprite {
public:
    static constexpr float WALK_SPEED = 0.2f;

    static constexpr float GRAVITY = 0.002f;

    static constexpr float GRAVITY_CAP = 0.8f;

    Player();
    Player(Graphics &graphics, Vector2 spawn_point);

    void draw(Graphics &graphics);
    void update(float time_elapsed);

    void move_left();
    void move_right();
    void stop_moving();

    virtual void on_animation_completed(std::string current_animation);
    virtual void setup_animations();

    const float get_position_x() const;
    const float get_position_y() const;

    void on_tile_collision(std::vector<Rectangle> &colliding_rectangles);

private:
    float delta_x;
    float delta_y;

    bool is_grounded;

    Direction facing;

    std::vector<Rectangle> colliding_rectangles;
};
