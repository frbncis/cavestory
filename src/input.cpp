#include <input.h>
#include <iostream>

void Input::begin_new_frame() {
    this->released_keys.clear();
    this->pressed_keys.clear();
}

void Input::key_down_handler(const SDL_Event& event) {
    std::cout << "Got key down _" << event.key.keysym.scancode << "_\n";

    this->pressed_keys[event.key.keysym.scancode] = true;
    this->held_keys[event.key.keysym.scancode] = true;
}

void Input::key_up_handler(const SDL_Event& event) {
    std::cout << "Got key up _" << event.key.keysym.scancode << "_\n";

    this->released_keys[event.key.keysym.scancode] = true;
    this->held_keys[event.key.keysym.scancode] = false;
}

bool Input::was_key_pressed(SDL_Scancode key) {
    return this->pressed_keys[key];
}

bool Input::was_key_released(SDL_Scancode key) {
    return this->released_keys[key];
}

bool Input::is_key_held(SDL_Scancode key) {
    return this->held_keys[key];
}