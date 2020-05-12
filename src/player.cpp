#include <animated_sprite.h>
#include <graphics.h>
#include <player.h>

namespace PlayerAnimations {
    const std::string IdleLeft = "IDLE_LEFT";
    const std::string IdleRight = "IDLE_RIGHT";
    const std::string RunLeft = "RUN_LEFT";
    const std::string RunRight = "RUN_RIGHT";
}

Player::Player() {
}

Player::Player(Graphics &graphics, float position_x, float position_y) :
    AnimatedSprite(
        graphics,
        "content/sprites/MyChar.png",
        0,
        0,
        16,
        16,
        position_x,
        position_y,
        150) {

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

void Player::setup_animations() {
    this->add_animation(1, 0, 0, PlayerAnimations::IdleLeft, 16, 16, Vector2(0, 0));
    this->add_animation(1, 0, 16, PlayerAnimations::IdleRight, 16, 16, Vector2(0, 0));
    this->add_animation(3, 0, 0, PlayerAnimations::RunLeft, 16, 16, Vector2(0, 0));
    this->add_animation(3, 0, 16, PlayerAnimations::RunRight, 16, 16, Vector2(0, 0));
}

void Player::update(float time_elapsed) {
    position_x += delta_x * time_elapsed;

    AnimatedSprite::update(time_elapsed);
}

void Player::draw(Graphics &graphics) {
    AnimatedSprite::draw(graphics, position_x, position_y);
}