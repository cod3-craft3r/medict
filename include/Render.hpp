#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "util.hpp"

// Screen dimensions
const int SCREEN_WIDTH = 854;
const int SCREEN_HEIGHT = 480;

// Structure to hold SDL resources
struct SDLContext {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    
    ~SDLContext() {
        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
        SDL_Quit();
    }
};

/** Helper function to load a texture from a path string
 * @param imagePath Path to the image file
 * @param renderer SDL Renderer
 * @return SDL_Texture pointer, or nullptr if loading fails
 */
SDL_Texture* loadTexture(const std::string &imagePath, SDL_Renderer* renderer);

/**
 * Initialize SDL2 and create a window with renderer
 * @param width Window width
 * @param height Window height
 * @param title Window title
 * @return Unique pointer to SDLContext, or nullptr if initialization fails
 */
std::unique_ptr<SDLContext> initializeSDL(const std::string& title);

/**
 * Load an image from a file path and render it in a window
 * @param imagePath Path to the image file
 * @param title Title of the rendered SDL window
 */
void Render(const std::string &imagePath, const std::string &title);
