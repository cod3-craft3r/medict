#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> // Required for PNG/JPG loading
#include <iostream>
#include <string>

// Screen dimensions
const int SCREEN_WIDTH = 854;
const int SCREEN_HEIGHT = 480;

// Helper function to load a texture from a path string
// This accepts the path you get from your "other object"
SDL_Texture* loadTexture(const std::string &path, SDL_Renderer* renderer) {
    // The final texture
    SDL_Texture* newTexture = nullptr;

    // Load image at specified path using SDL_image
    // IMG_LoadTexture creates a texture directly from a file
    newTexture = IMG_LoadTexture(renderer, path.c_str());

    if (newTexture == nullptr) {
        std::cout << "Failed to load image! SDL_image Error: " << IMG_GetError() << std::endl;
    }

    return newTexture;
}

int main(int argc, char* args[]) {
    // 1. Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // 2. Initialize SDL_image (IMPORTANT for PNG/JPG)
    // We explicitly ask to initialize PNG loading support
    int imgFlags = IMG_INIT_PNG; 
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return -1;
    }

    // 3. Create Window
    SDL_Window* window = SDL_CreateWindow("SDL2 Image Loader", 
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                          SCREEN_WIDTH, SCREEN_HEIGHT, 
                                          SDL_WINDOW_SHOWN);

    // 4. Create Renderer
    // This is the "painter" that draws to the window
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // --- YOUR LOGIC HERE ---
    // Simulate getting a path from another object
    std::string imagePaths[] = {"my_image.png", "su-30-mk1.jpg"};

    // Load the textures
    SDL_Texture* myTexture = loadTexture(imagePaths[0], renderer);
    // SDL_Texture* myTexture2 = loadTexture(imagePaths[1], renderer);
    // -----------------------

    // Force the cursor to appear
    SDL_ShowCursor(SDL_ENABLE);

    // Main loop flag
    bool quit = false;
    SDL_Event e;

    // 5. Game Loop
    while (!quit) {
        // Handle events (like clicking the X button)
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Clear screen (wipe previous frame)
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF); // White background
        SDL_RenderClear(renderer);

        // Render texture to screen
        if (myTexture != nullptr) {
            // NULL as the second argument means draw the whole image
            // NULL as the third argument means stretch it to fill the whole window
            // You can replace the 3rd NULL with an SDL_Rect to define size/position
            SDL_RenderCopy(renderer, myTexture, NULL, NULL);
        }

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // 6. Cleanup
    SDL_DestroyTexture(myTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
