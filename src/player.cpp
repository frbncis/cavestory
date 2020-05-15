#include <animated_sprite.h>
#include <graphics.h>
#include <player.h>

#include <iostream>

namespace PlayerAnimations {
    const std::string IdleLeft = "IDLE_LEFT";
    const std::string IdleRight = "IDLE_RIGHT";
    const std::string RunLeft = "RUN_LEFT";
    const std::string RunRight = "RUN_RIGHT";
}

Player::Player() {
}

Player::Player(Graphics &graphics, Vector2 spawn_point) :
    AnimatedSprite(
        graphics,
        "content/sprites/MyChar.png",
        0,
        0,
        16,
        16,
        spawn_point.x,
        spawn_point.y,
        150),
    delta_x(0),
    delta_y(0),
    facing(Right),
    is_grounded(false) {

    setup_animations();

    play_animation(PlayerAnimations::IdleRight);
};

void Player::on_animation_completed(std::string current_animation) {

}

void Player::move_left() {
    delta_x = -WALK_SPEED;
    play_animation(PlayerAnimations::RunLeft);
    facing = Direction::Left;
}

void Player::move_right() {
    delta_x = WALK_SPEED;
    play_animation(PlayerAnimations::RunRight);
    facing = Direction::Right;
}

void Player::stop_moving() {
    delta_x = 0;
    play_animation(facing == Direction::Right ? PlayerAnimations::IdleRight : PlayerAnimations::IdleLeft);
}

void Player::on_tile_collision(std::vector<Rectangle> &colliding_rectangles) {
    this->colliding_rectangles = colliding_rectangles;

    for (int i = 0; i < colliding_rectangles.size(); i++) {
        Rectangle colliding_rectangle = colliding_rectangles.at(i);

        sides::Side colliding_side = get_colliding_side(colliding_rectangle);

        switch (colliding_side) {
            case sides::Side::Top:
                position_y = colliding_rectangle.get_bottom() + 1;
                delta_y = 0;
                break;
            case sides::Side::Bottom:
                position_y = colliding_rectangle.get_top() - bounding_box.get_height() - 1;
                delta_y = 0;
                is_grounded = true;
                break;
            case sides::Side::Left:
                position_x = colliding_rectangle.get_right() + 1;
                break;
            case sides::Side::Right:
                position_x = colliding_rectangle.get_left() - bounding_box.get_width() - 1;
                break;
            default:
                break;
        }
    }
}

void Player::setup_animations() {
    this->add_animation(1, 0, 0, PlayerAnimations::IdleLeft, 16, 16, Vector2(0, 0));
    this->add_animation(1, 0, 16, PlayerAnimations::IdleRight, 16, 16, Vector2(0, 0));
    this->add_animation(3, 0, 0, PlayerAnimations::RunLeft, 16, 16, Vector2(0, 0));
    this->add_animation(3, 0, 16, PlayerAnimations::RunRight, 16, 16, Vector2(0, 0));
}

void Player::update(float time_elapsed) {
    // Update gravity
    if (delta_y <= GRAVITY_CAP) {
        delta_y += GRAVITY * time_elapsed;
    }

    position_x += delta_x * time_elapsed;
    position_y += delta_y * time_elapsed;

    AnimatedSprite::update(time_elapsed);
}

void Player::draw(Graphics &graphics) {
    AnimatedSprite::draw(graphics, position_x, position_y);

    bounding_box.draw(graphics, { 0, 255, 0, 255 });

    for (int i = 0; i < colliding_rectangles.size(); i++) {
        colliding_rectangles.at(i).draw(graphics, { 255, 0, 0, 255 });
    }
}

const float Player::get_position_x() const {
    return position_x;
}

const float Player::get_position_y() const {
    return position_y;
}