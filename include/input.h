#include <map>
#include <SDL2/SDL.h>

#pragma once

class Input {
public:
    /**
     * Method gets called at the beginning of each
     * new frame to reset the keys that are no longer relevant.
     */
    void begin_new_frame();

    void key_up_handler(const SDL_Event& event);
    void key_down_handler(const SDL_Event& event);

    bool was_key_pressed(SDL_Scancode key);
    bool was_key_released(SDL_Scancode key);
    bool is_key_held(SDL_Scancode key);

private:
    std::map<SDL_Scancode, bool> held_keys;
    std::map<SDL_Scancode, bool> pressed_keys;
    std::map<SDL_Scancode, bool> released_keys;
};