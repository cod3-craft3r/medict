#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
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
 * Load images from file paths and render them in a window.
 * Supports navigation with Left/Right arrow keys.
 * @param imagePaths Vector of paths to the image files
 * @param title Title of the rendered SDL window
 */
void Render(const std::vector<std::string> &imagePaths, const std::string &title);

// Non-blocking request to open a window with a gallery of images
void requestRender(const std::vector<std::string>& imagePaths, const std::string& title);

// The main UI loop (runs on Main Thread)
void runRenderLoop(std::atomic<bool>& isAppRunning);
