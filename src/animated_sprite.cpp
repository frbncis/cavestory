#include <animated_sprite.h>
#include <graphics.h>

AnimatedSprite::AnimatedSprite() { }

AnimatedSprite::AnimatedSprite(
    Graphics &graphics,
    const std::string &file_path,
    int source_x,
    int source_y,
    int width,
    int height,
    float position_x,
    float position_y,
    float sprite_frame_period) :
        Sprite(graphics, file_path, source_x, source_y, width, height, position_x, position_y),
        frame_index(0),
        sprite_frame_period(sprite_frame_period),
        visible(true),
        current_animation_once(false),
        current_animation("")
        {
        }

void AnimatedSprite::add_animation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset) {
    std::vector<SDL_Rect> rectangles;

    // READ: Can I use a range operator?
    for (int i = 0; i < frames; i++) {
        SDL_Rect rectangle = { (i+x) * width, y, width, height };

        rectangles.push_back(rectangle);
    }

    this->animations.insert(std::pair<std::string, std::vector<SDL_Rect>>(name, rectangles));
    this->offsets.insert(std::pair<std::string, Vector2>(name, offset));
}

void AnimatedSprite::reset_animations() {
    this->animations.clear();
    this->offsets.clear();
}


void AnimatedSprite::play_animation(std::string animation, bool once) {
    this->current_animation_once = once;

    if (this->current_animation != animation) {
        this->current_animation = animation;
        this->frame_index = 0;
    }
}

void AnimatedSprite::set_visible(bool visible) {
    this->visible = visible;
}

void AnimatedSprite::stop_animation() {
    this->frame_index = 0;
    this->on_animation_completed(this->current_animation);
}

void AnimatedSprite::update(int time_elapsed) {
    Sprite::update();

    time_since_sprite_frame_changed += time_elapsed;

    if (time_since_sprite_frame_changed > sprite_frame_period) {
        if (frame_index < animations[current_animation].size() -1) {
            time_since_sprite_frame_changed = 0;
            frame_index++;
        } else {
            if (current_animation_once) {
                set_visible(false);
            }

            frame_index = 0;

            on_animation_completed(current_animation);
        }
    }
}

void AnimatedSprite::draw(Graphics &graphics, int x, int y) {
    // TODO: This feels odd, there's too many source_rect* here...
    if (visible) {
        SDL_Rect destination_rectangle;
        SDL_Rect source_rectangle = animations[current_animation][frame_index];

        destination_rectangle.x = x + offsets[current_animation].x;
        destination_rectangle.y = y + offsets[current_animation].y;

        destination_rectangle.w = this->source_rect.w * graphics.sprite_scale_factor;
        destination_rectangle.h = this->source_rect.h * graphics.sprite_scale_factor;

        graphics.blit_surface(this->sprite_sheet, &source_rectangle, &destination_rectangle);
    }
}

void AnimatedSprite::on_animation_completed(std::string current_animation) {

}

void AnimatedSprite::setup_animations() {
    this->add_animation(3, 0, 0, "RUN_LEFT", 16, 16, Vector2(0, 0));
    this->add_animation(3, 0, 16, "RUN_RIGHT", 16, 16, Vector2(0, 0));
}