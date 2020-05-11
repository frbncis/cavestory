#include <map>
#include <SDL2/SDL.h>
#include <string>

#pragma once

struct SDL_Window;
struct SDL_Renderer;

class Graphics {
public:
    int screen_width;
    int screen_height;
    int sprite_scale_factor;

    Graphics();
    ~Graphics();

    /**
     * Loads an image into the sprite sheet map if it doesn't already exist.
     * 
     * Returns the image from the map.
     */
    SDL_Surface* load_image(const std::string &file_path);

    /**
     * Draws a texture to a part of the screem denoted by destination_rectangle
     */
    void blit_surface(SDL_Texture* source, SDL_Rect* source_rectangle, SDL_Rect* destination_rectangle);

    /**
     * Draws everything to the screen.
     */
    void flip();

    /**
     * Clears the screen.
     */
    void clear();

    SDL_Renderer* get_renderer();
private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    std::map<std::string, SDL_Surface*> sprite_sheets;
};