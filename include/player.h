#include<animated_sprite.h>
#include<direction.h>

#pragma once

class Player : public AnimatedSprite {
public:
    static constexpr float WALK_SPEED = 0.2f;

    Player();
    Player(Graphics &graphics, float position_x, float position_y);

    void draw(Graphics &graphics);
    void update(float time_elapsed);

    void move_left();
    void move_right();
    void stop_moving();

    virtual void on_animation_completed(std::string current_animation);
    virtual void setup_animations();

private:
    float delta_x;
    float delta_y;

    Direction facing; 
};
