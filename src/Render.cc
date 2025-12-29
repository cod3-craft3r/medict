#include "Render.hpp"

SDL_Texture* loadTexture(const std::string &imagePath, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = nullptr;

    // Load image at specified path using SDL_image
    // IMG_LoadTexture creates a texture directly from a file
    newTexture = IMG_LoadTexture(renderer, imagePath.c_str());

    if (newTexture == nullptr) {
        std::cerr << "Failed to load image! SDL_image Error: " << IMG_GetError() << std::endl;
    }

    return newTexture;
}

std::unique_ptr<SDLContext> initializeSDL(const std::string& title) {
    // 1. Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }
    
    auto context = std::make_unique<SDLContext>();

    // 2. Create Window
    context->window = SDL_CreateWindow(title.c_str(),
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                          SCREEN_WIDTH, SCREEN_HEIGHT, 
                                          SDL_WINDOW_SHOWN);
    
    if (!context->window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return nullptr;
    }
    
    // 3. Create Renderer
    // This is the "painter" that draws to the window
    context->renderer = SDL_CreateRenderer(context->window, -1, SDL_RENDERER_ACCELERATED);
    
    if (!context->renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(context->window);
        SDL_Quit();
        return nullptr;
    }

    // 4. Initialize SDL_image (IMPORTANT for PNG/JPG)
    int imgFlags = (IMG_INIT_PNG | IMG_INIT_JPG);
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(context->renderer);
        SDL_DestroyWindow(context->window);
        SDL_Quit();
        return nullptr;
    }
    
    return context;
}

void Render(const std::string imagePath, const std::string title) {

    std::unique_ptr<SDLContext> ctx = initializeSDL(title);

    // Load the texture
    SDL_Texture* imgTexture = loadTexture(imagePath, ctx->renderer);
    
    // Force the cursor to appear
    SDL_ShowCursor(SDL_ENABLE);

    // 5. Game Loop
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        // Handle events (like clicking the X button)
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Clear screen (wipe previous frame)
        SDL_SetRenderDrawColor(ctx->renderer, 0xFF, 0xFF, 0xFF, 0xFF); // White background
        SDL_RenderClear(ctx->renderer);

        // Render texture to screen
        if (imgTexture != nullptr) {
            // NULL as the second argument means draw the whole image
            // NULL as the third argument means stretch it to fill the whole window
            // You can replace the 3rd NULL with an SDL_Rect to define size/position
            SDL_RenderCopy(ctx->renderer, imgTexture, NULL, NULL);
        }

        // Update screen
        SDL_RenderPresent(ctx->renderer);
    }

    // 6. Cleanup
    SDL_DestroyTexture(imgTexture);
    SDL_DestroyRenderer(ctx->renderer);
    SDL_DestroyWindow(ctx->window);
    IMG_Quit();
    SDL_Quit();
}
